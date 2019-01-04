import (
	"encoding/binary"
	"os"
)

func read_pcapng(fl string) [][]byte {
	res := [][]byte{}
	f, err := os.Open(fl)
	if err != nil {
		return res
	}
	defer f.Close()
	tmp := make([]byte, 4)
	for {
		_, err = f.Read(tmp)
		if err != nil {
			break
		}
		k := binary.LittleEndian.Uint32(tmp)
		_, err = f.Read(tmp)
		if err != nil {
			break
		}
		l := binary.LittleEndian.Uint32(tmp)
		frame := make([]byte, l-12)
		_, err = f.Read(frame)
		if err != nil {
			break
		}
		_, err = f.Read(tmp)
		if err != nil {
			break
		}
		if k == 6 {
			sz := binary.LittleEndian.Uint32(frame[16:20])
			data := make([]byte, sz)
			copy(data, frame[20:20+sz])
			res = append(res, data)
		}
	}
	return res
}
