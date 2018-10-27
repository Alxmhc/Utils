import conv

def read(fl):
	while True:
		h = fl.read(4)
		if not h:
			break
		type = conv.bytes_int_le(h)
		h = fl.read(4)
		len = conv.bytes_int_le(h)
		b = fl.read(len-12)
		fl.read(4)
		if type==6:
			r = {}
			sz = conv.bytes_int_le(b[12:16])
			r['size_lost'] = conv.bytes_int_le(b[16:20]) - sz
			r['frame'] = b[20:20+sz]
			yield r
