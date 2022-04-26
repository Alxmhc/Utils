package fl_zip

import (
	_crypt "_/crypt"
	"compress/flate"
	"io/ioutil"
	"os"

	"bytes"
	"crypto/aes"
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

type inf struct {
	Dpos  int64
	Dsize uint32

	Fname string
	Fsize uint32
	CRC32 [4]byte

	Method     uint16
	Encryption byte
}

type Fl struct {
	rd    io.ReadSeeker
	F_inf []inf
	psw   []byte
}

func (s *Fl) readInf() {
	sz, _ := s.rd.Seek(0, io.SeekEnd)
	s.rd.Seek(0, io.SeekStart)
	for {
		h := make([]byte, 4)
		n, _ := s.rd.Read(h)
		if n != 4 {
			break
		}
		if !bytes.Equal(h, []byte{0x50, 0x4b, 0x03, 0x04}) {
			break
		}
		hdr := make([]byte, 26)
		n, _ = s.rd.Read(hdr)
		if n != 26 {
			break
		}
		var res inf
		res.Encryption = hdr[2] & 1
		res.Method = binary.LittleEndian.Uint16(hdr[4:6])
		copy(res.CRC32[:], hdr[10:14])
		res.Dsize = binary.LittleEndian.Uint32(hdr[14:18])
		res.Fsize = binary.LittleEndian.Uint32(hdr[18:22])

		szfn := binary.LittleEndian.Uint16(hdr[22:24])
		fns := make([]byte, szfn)
		n, _ = s.rd.Read(fns)
		if uint16(n) != szfn {
			break
		}
		res.Fname = string(fns)

		szex := binary.LittleEndian.Uint16(hdr[24:26])
		if szex != 0 {
			ext := make([]byte, szex)
			n, _ = s.rd.Read(ext)
			if uint16(n) != szex {
				break
			}
			if res.Method == 99 {
				res.Encryption = ext[8] + 1
				res.Method = binary.LittleEndian.Uint16(ext[9:11])
			}
		}
		res.Dpos, _ = s.rd.Seek(0, io.SeekCurrent)
		n64, _ := s.rd.Seek(int64(res.Dsize), io.SeekCurrent)
		if sz < n64 {
			break
		}
		s.F_inf = append(s.F_inf, res)
	}
}

func updKey(key []uint32, c byte) {
	key[0] = crc32.IEEETable[byte(key[0])^c] ^ (key[0] >> 8)
	key[1] = (key[1]+(key[0]&0xff))*0x8088405 + 1
	key[2] = crc32.IEEETable[byte(key[2]^(key[1]>>24))] ^ (key[2] >> 8)
}

func (s *Fl) decryptZIP(d []byte) []byte {
	if len(s.psw) == 0 || len(d) < 12 {
		return nil
	}

	key := [3]uint32{0x12345678, 0x23456789, 0x34567890}
	for _, c := range s.psw {
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

func (v *vle) Init() {
	copy(v.data[:], []byte{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0})
}
func (v *vle) Data() []byte {
	return v.data[:]
}
func (v *vle) Incr() {
	for i := 0; i < 16; i++ {
		if v.data[i] != 255 {
			v.data[i]++
			break
		}
		v.data[i] = 0
	}
}

func (s *Fl) decryptAES(d []byte, saltLen int) []byte {
	if len(s.psw) == 0 {
		return nil
	}

	salt := d[:saltLen]
	keyLen := saltLen * 2
	key := _crypt.PBKDF2HMAC(s.psw, salt, 1000, (keyLen*2)+2, sha1.New)

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
	cr, _ := aes.NewCipher(key[:keyLen])
	_crypt.CR_CTR_Decrypt(cr, &vle{}, data)
	return data
}

func (s *Fl) Open(pth string) {
	var err error
	s.rd, err = os.Open(pth)
	if err != nil {
		return
	}
	s.readInf()
}

func (s *Fl) Fnames() []string {
	var res []string
	for _, inf := range s.F_inf {
		res = append(res, inf.Fname)
	}
	return res
}

func (s *Fl) SetPsw(passw []byte) {
	s.psw = make([]byte, len(passw))
	copy(s.psw[:], passw)
}

func (s *Fl) getData(n uint) []byte {
	data := make([]byte, s.F_inf[n].Dsize)
	s.rd.Seek(s.F_inf[n].Dpos, io.SeekStart)
	s.rd.Read(data)
	return data
}

func (s *Fl) GetData(n uint) []byte {
	data := s.getData(n)
	switch s.F_inf[n].Encryption {
	case eNO:
	case eZIP:
		data = s.decryptZIP(data)
	case eAES128:
		data = s.decryptAES(data, 8)
	case eAES192:
		data = s.decryptAES(data, 12)
	case eAES256:
		data = s.decryptAES(data, 16)
	default:
		data = nil
	}
	switch s.F_inf[n].Method {
	case cNO:
	case cDeflate:
		rd := flate.NewReader(bytes.NewReader(data))
		data, _ = ioutil.ReadAll(rd)
	default:
		data = nil
	}
	return data
}
