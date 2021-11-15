package data

func LZ77_repeat(sz, dist uint, d []byte) []byte {
	if dist == 0 || dist > uint(len(d)) {
		return nil
	}
	if sz == 0 {
		return d
	}
	beg := uint(len(d)) - dist
	for sz > dist {
		d = append(d, d[beg:]...)
		sz -= dist
		dist += dist
	}
	return append(d, d[beg:beg+sz]...)
}
