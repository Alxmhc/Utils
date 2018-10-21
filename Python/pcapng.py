def read(fl):
	while True:
		h = fl.read(4)
		if not h:
			break
		type = bytes_int_le(h)
		h = fl.read(4)
		len = bytes_int_le(h)
		b = fl.read(len-12)
		fl.read(4)
		if type==6:
			r = {}
			sz = bytes_int_le(b[12:16])
			r['size_lost'] = bytes_int_le(b[16:20]) - sz
			r['frame'] = b[20:20+sz]
			yield r
