def read(fl):
	while True:
		b = fl.read(4)
		if b == b'':
			break
		type = int.from_bytes(b, byteorder='little')
		b = fl.read(4)
		len = int.from_bytes(b, byteorder='little')
		b = fl.read(len-12)
		fl.read(4)
		if type==6:
			r = {}
			sz = int.from_bytes(b[12:16], byteorder='little')
			r['size_lost'] = int.from_bytes(b[16:20], byteorder='little') - sz
			r['frame'] = b[20:20+sz]
			yield r
