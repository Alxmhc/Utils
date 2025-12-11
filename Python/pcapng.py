def reader(pth):
	with open(pth, "rb") as fl:
		while True:
			b = fl.read(4)
			if not b:
				break
			t = int.from_bytes(b, byteorder='little')
			b = fl.read(4)
			if not b:
				break
			size = int.from_bytes(b, byteorder='little')
			if size < 12:
				break
			bl = fl.read(size - 12)
			if not bl:
				break
			b = fl.read(4)
			if not b:
				break
			yield [t, bl]
		yield None

def parse(p):
	t, b = p
	res = {'type':t}
	sz = len(b)
	match t:
		case 6:
			if sz < 20:
				return res
			size = int.from_bytes(b[12:16], byteorder='little')
			if sz < size + 20:
				return res
			res['ID'] = int.from_bytes(b[:4], byteorder='little')
			res['Time'] = b[4:12]
			res['osz'] = int.from_bytes(b[16:20], byteorder='little')
			res['body'] = b[20:20+size]
		case _:
			res['body'] = b
	return res
