package parsefile

import (
	"bytes"
	"encoding/binary"
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
	eNO      = 0
	eUNKNOWN = 1
	eAES128  = 2
	eAES192  = 3
	eAES256  = 4
)

type Infzip struct {
	Encryption byte
	Fname      string
	Method     uint16
	CRC32      [4]byte
	Fsize      uint32
	Psize      uint32
	Ppos       uint64
}

func readHdr(rs io.ReadSeeker) *Infzip {
	hdr := make([]byte, 26)
	_, err := rs.Read(hdr)
	if err != nil {
		return nil
	}
	var res Infzip
	{
		szfn := binary.LittleEndian.Uint16(hdr[22:24])
		fns := make([]byte, szfn)
		_, err = rs.Read(fns)
		if err != nil {
			return nil
		}
		res.Fname = string(fns)
	}
	res.Encryption = hdr[2] & 1
	res.Method = binary.LittleEndian.Uint16(hdr[4:6])
	copy(res.CRC32[:], hdr[10:14])
	res.Fsize = binary.LittleEndian.Uint32(hdr[18:22])
	res.Psize = binary.LittleEndian.Uint32(hdr[14:18])
	szex := binary.LittleEndian.Uint16(hdr[24:26])
	if szex != 0 {
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
	offset, _ := rs.Seek(0, io.SeekCurrent)
	res.Ppos = uint64(offset)
	return &res
}

func ReadInfzip(rs io.ReadSeeker) []Infzip {
	var res []Infzip
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
			rs.Seek(int64(inf.Psize), io.SeekCurrent)
		} else {
			break
		}
	}
	return res
}

type InfzipEncr struct {
	Salt []byte
	Psv  [2]byte
	Auth [10]byte
}

func ReadInfzipEncr(rs io.ReadSeeker, inf Infzip) *InfzipEncr {
	ssize := 0
	switch inf.Encryption {
	case eAES128:
		ssize = 8
	case eAES192:
		ssize = 12
	case eAES256:
		ssize = 16
	}
	if ssize == 0 {
		return nil
	}
	var res InfzipEncr
	res.Salt = make([]byte, ssize)
	rs.Read(res.Salt)
	rs.Read(res.Psv[:])
	rs.Seek(int64(inf.Psize)-int64(ssize)-12, io.SeekCurrent)
	rs.Read(res.Auth[:])
	return &res
}
