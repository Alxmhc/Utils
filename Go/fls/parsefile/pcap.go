package parsefile

import "encoding/binary"

type RDpcap struct {
	readerFC
}

func (r *RDpcap) Init(fl string) {
	r.open(fl)
	if r.fl == nil {
		return
	}
	tmp := make([]byte, 24)
	_, err := r.fl.Read(tmp)
	if err != nil {
		r.Close()
	}
}

func (r *RDpcap) Read() []byte {
	if r.fl == nil {
		return nil
	}
	tmp := make([]byte, 16)
	_, err := r.fl.Read(tmp)
	if err != nil {
		r.Close()
		return nil
	}
	sz := binary.LittleEndian.Uint32(tmp[8:12])
	data := make([]byte, sz)
	_, err = r.fl.Read(data)
	if err != nil {
		r.Close()
		return nil
	}
	return data
}
