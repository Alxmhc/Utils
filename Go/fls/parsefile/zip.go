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
	Hpos  uint64
	Hsize uint16

	Encryption byte
	Method     uint16
	CRC32      [4]byte
	Dsize      uint32
	Fsize      uint32
	Fname      string
}

func readHdr(rs io.ReadSeeker) *Infzip {
	hdr := make([]byte, 26)
	_, err := rs.Read(hdr)
	if err != nil {
		return nil
	}
	var res Infzip
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
			rs.Seek(int64(inf.Dsize), io.SeekCurrent)
		} else {
			break
		}
	}
	return res
}
