package fl_zip

import (
	_crypt "_/crypt"

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
	Dpos  uint64
	Dsize uint32

	Fname string
	Fsize uint32
	CRC32 [4]byte

	Method     uint16
	Encryption byte
	IsDir      bool
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
	res.Dpos = res.Hpos + uint64(res.Hsize)
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
			inf.IsDir = (inf.Fname[len(inf.Fname)-1] == '/')
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

func decryptZIP(d []byte, passw []byte) []byte {
	key := [3]uint32{0x12345678, 0x23456789, 0x34567890}
	for _, c := range passw {
		updKey(key[:], c)
	}

	for i := 0; i < len(d); i++ {
		tmp := key[2] | 2
		d[i] ^= byte((tmp * (tmp ^ 1)) >> 8)
		updKey(key[:], d[i])
	}
	return d[12:]
}

type vle struct {
	data [16]byte
}

func (v *vle) Data() []byte {
	return v.data[:]
}
func (v *vle) Incr() {
	for i := 0; i < 16; i++ {
		v.data[i]++
		if v.data[i] != 0 {
			break
		}
	}
}

func decryptAES(d []byte, passw []byte, saltLen int) []byte {
	salt := d[:saltLen]
	keyLen := saltLen * 2
	key := _crypt.PBKDF2HMAC(passw, salt, 1000, (keyLen*2)+2, sha1.New)

	psv := d[saltLen : saltLen+2]
	if !bytes.Equal(psv, key[keyLen*2:]) {
		return nil
	}

	data := d[saltLen+2 : len(d)-10]
	mac := hmac.New(sha1.New, key[keyLen:keyLen*2])
	mac.Write(data)
	eauth := mac.Sum(nil)

	auth := d[len(d)-10:]
	if !bytes.Equal(eauth[:10], auth) {
		return nil
	}
	_crypt.AESctrDecrypt(data, key[:keyLen], &vle{})
	return data
}

func Decrypt(f io.ReaderAt, inf Inf, passw []byte) []byte {
	data := make([]byte, inf.Dsize)
	f.ReadAt(data, int64(inf.Dpos))
	switch inf.Encryption {
	case eZIP:
		return decryptZIP(data, passw)
	case eAES128:
		return decryptAES(data, passw, 8)
	case eAES192:
		return decryptAES(data, passw, 12)
	case eAES256:
		return decryptAES(data, passw, 16)
	}
	return nil
}
