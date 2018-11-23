def unchunk(b):
	r = b''
	while True:
		e = b.find(b'\r')
		if(e == -1):
			break
		sz = int(b[:e], 16)
		if(sz == 0):
			break
		e += 2
		r += b[e:e+sz]
		b = b[e+sz+2:]
	return r
