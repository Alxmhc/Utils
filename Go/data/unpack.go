package data

func LZ77_repeat(sz, dist int, d []byte) []byte {
	osz := len(d)
	if dist == 0 || osz < dist {
		return nil
	}
	beg := osz - dist
	for sz > dist {
		d = append(d, d[beg:]...)
		sz -= dist
		dist += dist
	}
	if sz == 0 {
		return d
	}
	return append(d, d[beg:beg+sz]...)
}
