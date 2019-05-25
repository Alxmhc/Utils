def read(fl):
	fl.read(24)
	while True:
		b = fl.read(16)
		if not b:
			break
		sz = int.from_bytes(b[8:12], byteorder='little')
		r = {}
		r['size_lost'] = int.from_bytes(b[12:16], byteorder='little') - sz
		r['frame'] = fl.read(sz)
		yield r
