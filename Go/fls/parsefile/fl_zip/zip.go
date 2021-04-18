package fl_zip

import (
	"_/crypt"

	"bytes"
	"crypto/hmac"
	"crypto/sha1"
	"encoding/binary"
	"hash/crc32"
	"io"
)

const (
	cNO        = 0
	cDeflate   = 8
	cDeflate64 = 9
	cBZIP2     = 12
	cLZMA      = 14
	cPPMd      = 98
)

const (
	eNO     = 0
	eZIP    = 1
	eAES128 = 2
	eAES192 = 3
	eAES256 = 4
)

type Inf struct {
	Hpos  uint64
	Hsize uint16

	Encryption byte
	Method     uint16
	CRC32      [4]byte
	Dsize      uint32
	Fsize      uint32
	Fname      string
}

func readHdr(rs io.ReadSeeker) *Inf {
	hdr := make([]byte, 26)
	_, err := rs.Read(hdr)
	if err != nil {
		return nil
	}
	var res Inf
	offset, _ := rs.Seek(0, io.SeekCurrent)
	res.Hpos = uint64(offset - 26)
	res.Hsize = 26

	res.Encryption = hdr[2] & 1
	res.Method = binary.LittleEndian.Uint16(hdr[4:6])
	copy(res.CRC32[:], hdr[10:14])
	res.Dsize = binary.LittleEndian.Uint32(hdr[14:18])
	res.Fsize = binary.LittleEndian.Uint32(hdr[18:22])

	szfn := binary.LittleEndian.Uint16(hdr[22:24])
	res.Hsize += szfn
	fns := make([]byte, szfn)
	_, err = rs.Read(fns)
	if err != nil {
		return nil
	}
	res.Fname = string(fns)

	szex := binary.LittleEndian.Uint16(hdr[24:26])
	if szex != 0 {
		res.Hsize += szex
		ext := make([]byte, szex)
		_, err = rs.Read(ext)
		if err != nil {
			return nil
		}
		if res.Method == 99 {
			res.Encryption = ext[8] + 1
			res.Method = binary.LittleEndian.Uint16(ext[9:11])
		}
	}
	return &res
}

func ReadInf(rs io.ReadSeeker) []Inf {
	var res []Inf
	hdr := make([]byte, 4)
	for {
		_, err := rs.Read(hdr)
		if err != nil {
			break
		}
		if bytes.Equal(hdr, []byte{0x50, 0x4b, 0x03, 0x04}) {
			inf := readHdr(rs)
			if inf == nil {
				break
			}
			//folder
			if inf.Fname[len(inf.Fname)-1] == '/' {
				continue
			}
			res = append(res, *inf)
			rs.Seek(int64(inf.Dsize), io.SeekCurrent)
		} else {
			break
		}
	}
	return res
}

func updKey(key []uint32, c byte) {
	key[0] = crc32.IEEETable[byte(key[0])^c] ^ (key[0] >> 8)
	key[1] = (key[1]+(key[0]&0xff))*0x8088405 + 1
	key[2] = crc32.IEEETable[byte(key[2]^(key[1]>>24))] ^ (key[2] >> 8)
}

func decryptZIP(f io.ReaderAt, inf Inf, passw []byte) []byte {
	r := io.NewSectionReader(f, int64(inf.Hpos)+int64(inf.Hsize), int64(inf.Dsize))
	data := make([]byte, inf.Dsize)
	_, err := r.Read(data)
	if err != nil {
		return nil
	}

	key := [3]uint32{0x12345678, 0x23456789, 0x34567890}
	for _, c := range passw {
		updKey(key[:], c)
	}

	for i := 0; i < len(data); i++ {
		tmp := key[2] | 2
		data[i] ^= byte((tmp * (tmp ^ 1)) >> 8)
		updKey(key[:], data[i])
	}
	return data[12:]
}

func decryptAES(f io.ReaderAt, inf Inf, saltLen int, passw []byte) []byte {
	r := io.NewSectionReader(f, int64(inf.Hpos)+int64(inf.Hsize), int64(inf.Dsize))
	salt := make([]byte, saltLen)
	r.Read(salt)
	var psv [2]byte
	r.Read(psv[:])

	keyLen := saltLen * 2
	key := crypt.PBKDF2HMAC(passw, salt, 1000, (keyLen*2)+2, sha1.New)
	if !bytes.Equal(psv[:], key[keyLen*2:]) {
		return nil
	}

	data := make([]byte, inf.Dsize-uint32(saltLen)-12)
	_, err := r.Read(data)
	if err != nil {
		return nil
	}

	var auth [10]byte
	r.Read(auth[:])
	mac := hmac.New(sha1.New, key[keyLen:keyLen*2])
	mac.Write(data)
	eauth := mac.Sum(nil)
	if !bytes.Equal(eauth[:10], auth[:]) {
		return nil
	}

	crypt.AESCTRDecryptLE(data, key[:keyLen])
	return data
}

func Decrypt(f io.ReaderAt, inf Inf, passw []byte) []byte {
	switch inf.Encryption {
	case eZIP:
		return decryptZIP(f, inf, passw)
	case eAES128:
		return decryptAES(f, inf, 8, passw)
	case eAES192:
		return decryptAES(f, inf, 12, passw)
	case eAES256:
		return decryptAES(f, inf, 16, passw)
	}
	return nil
}
