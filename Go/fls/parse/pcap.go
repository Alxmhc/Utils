import (
	"encoding/binary"
	"os"
)

func read_pcap(fl string) [][]byte {
	res := [][]byte{}
	f, err := os.Open(fl)
	if err != nil {
		return res
	}
	defer f.Close()
	{
		tmp := make([]byte, 24)
		_, err := f.Read(tmp)
		if err != nil {
			return res
		}
	}
	tmp := make([]byte, 16)
	for {
		_, err = f.Read(tmp)
		if err != nil {
			break
		}
		sz := binary.LittleEndian.Uint32(tmp[8:12])
		data := make([]byte, sz)
		_, err = f.Read(data)
		if err != nil {
			break
		}
		res = append(res, data)
	}
	return res
}
