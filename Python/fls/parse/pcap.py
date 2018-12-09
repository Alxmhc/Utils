import conv

def read(fl):
	fl.read(24)
	while True:
		b = fl.read(16)
		if not b:
			break
		sz = conv.bytes_int_le(b[8:12])
		r = {}
		r['size_lost'] = conv.bytes_int_le(b[12:16]) - sz
		r['frame'] = fl.read(sz)
		yield r
