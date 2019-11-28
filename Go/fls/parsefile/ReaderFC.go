package parsefile

import "os"

type readerFC struct {
	fl *os.File
}

func (r *readerFC) open(fl string) {
	r.Close()
	r.fl, _ = os.Open(fl)
}

func (r *readerFC) Close() {
	if r.fl != nil {
		r.fl.Close()
		r.fl = nil
	}
}
