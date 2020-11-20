package parsefile

import (
	"bytes"
	"encoding/binary"
	"io"
	"os"
)

type Infzip struct {
	Encription byte
	Fname      string
	Method     uint16
	CRC32      [4]byte
	Fsize      uint32
	Psize      uint32
	Ppos       uint64
}

func readHdr(fl os.File) *Infzip {
	hdr := make([]byte, 26)
	_, err := fl.Read(hdr)
	if err != nil {
		return nil
	}
	var res Infzip
	{
		szfn := binary.LittleEndian.Uint16(hdr[22:24])
		fns := make([]byte, szfn)
		_, err = fl.Read(fns)
		if err != nil {
			return nil
		}
		res.Fname = string(fns)
	}
	res.Encription = hdr[2] & 1
	res.Method = binary.LittleEndian.Uint16(hdr[4:6])
	copy(res.CRC32[:], hdr[10:14])
	res.Fsize = binary.LittleEndian.Uint32(hdr[18:22])
	res.Psize = binary.LittleEndian.Uint32(hdr[14:18])
	szex := binary.LittleEndian.Uint16(hdr[24:26])
	if szex != 0 {
		ext := make([]byte, szex)
		_, err = fl.Read(ext)
		if err != nil {
			return nil
		}
		if res.Method == 99 {
			res.Encription = ext[8] + 1
			res.Method = binary.LittleEndian.Uint16(ext[9:11])
		}
	}
	offset, _ := fl.Seek(0, io.SeekCurrent)
	res.Ppos = uint64(offset)
	return &res
}

func ReadInfzip(fl os.File) []Infzip {
	var res []Infzip
	hdr := make([]byte, 4)
	for {
		_, err := fl.Read(hdr)
		if err != nil {
			break
		}
		if bytes.Equal(hdr, []byte{0x50, 0x4b, 0x03, 0x04}) {
			inf := readHdr(fl)
			if inf == nil {
				break
			}
			//folder
			if inf.Fname[len(inf.Fname)-1] == '/' {
				continue
			}
			res = append(res, *inf)
			fl.Seek(int64(inf.Psize), io.SeekCurrent)
		} else {
			break
		}
	}
	return res
}
