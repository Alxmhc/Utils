package data

func Unpack1LE(m []uint32) []byte {
	r := make([]byte, len(m)<<2)
	for k,c := range m {
		r[k<<2] = byte(c)
		r[k<<2+1] = byte(c>>8)
		r[k<<2+2] = byte(c>>16)
		r[k<<2+3] = byte(c>>24)
	}
	return r
}

func Unpack1BE(m []uint32) []byte {
	r := make([]byte, len(m)<<2)
	for k,c := range m {
		r[k<<2] = byte(c>>24)
		r[k<<2+1] = byte(c>>16)
		r[k<<2+2] = byte(c>>8)
		r[k<<2+3] = byte(c)
	}
	return r
}
