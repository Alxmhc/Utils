def read(fl):
	fl.seek(24, 1)
	while True:
		b = fl.read(16)
		if b == b'':
			break
		sz = int.from_bytes(b[8:12], byteorder='little')
		r = {}
		r['size_lost'] = int.from_bytes(b[12:16], byteorder='little') - sz
		r['frame'] = fl.read(sz)
		yield r
