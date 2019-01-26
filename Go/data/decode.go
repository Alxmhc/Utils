package data

import (
	"bytes"
	"strconv"
)

func Unchunk(v []byte) []byte {
	var res []byte
	for true {
		n := bytes.Index(v, []byte("\r\n"))
		if n == -1 {
			break
		}
		sz,err := strconv.ParseInt(string(v[:n]), 16, 64)
		if sz == 0 || err != nil {
			break
		}
		n = n + 2
		sz = sz + int64(n)
		res = append(res, v[n:sz]...)
		v = v[sz+2:]
	}
	return res
}
