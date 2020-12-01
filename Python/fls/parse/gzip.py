def readStr(fl):
	s = bytearray()
	while True:
		c = fl.read(1)
		if c == b'\x00':
			break
		s += c
	return s.decode("utf-8")

def read_inf(fl):
	res = {}
	hdr = fl.read(3)
	if hdr != b"\x1f\x8b\x08":
		return res
	flg = ord( fl.read(1) )
	if flg > 31:
		return res
	fl.seek(6, 1)
	if (flg & 4) != 0:
		bsz = fl.read(2)
		sz = int.from_bytes(bsz, byteorder='little')
		fl.seek(sz, 1)
	if (flg & 8) != 0:
		res["name"] = readStr(fl)
	if (flg & 16) != 0:
		res["comment"] = readStr(fl)
	if (flg & 2) != 0:
		fl.seek(2, 1)
	st = fl.tell()
	fl.seek(-8, 2)
	end = fl.tell()
	res["ppos"] = st
	res["psize"] = end - st
	res["crc32"] = fl.read(4)
	bsz = fl.read(4)
	res["fsize"] = int.from_bytes(bsz, byteorder='little')
	return res
