package parsefile

import "encoding/binary"

type RDpcapng struct {
	readerFC
}

func (r *RDpcapng) Init(fl string){
	r.open(fl)
}

func (r *RDpcapng) Read() []byte {
	if r.fl == nil {
		return nil
	}
	tmp := make([]byte, 4)
	for {
		_, err := r.fl.Read(tmp)
		if err != nil {
			break
		}
		k := binary.LittleEndian.Uint32(tmp)
		_, err = r.fl.Read(tmp)
		if err != nil {
			break
		}
		l := binary.LittleEndian.Uint32(tmp)
		frame := make([]byte, l-12)
		_, err = r.fl.Read(frame)
		if err != nil {
			break
		}
		_, err = r.fl.Read(tmp)
		if err != nil {
			break
		}
		if k == 6 {
			sz := binary.LittleEndian.Uint32(frame[16:20])
			return frame[20:20+sz]
		}
	}
	r.Close()
	return nil
}
