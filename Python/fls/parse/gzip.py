def read_string(fl):
	str = bytearray(b'')
	while True:
		c = fl.read(1)[0]
		if c == 0:
			return str
		str.append(c)

def readHdr(fl):
	hdr = fl.read(3)
	if(hdr[0] != 0x1f or hdr[1] != 0x8b or hdr[2] != 0x08):
		return None
	flg = fl.read(1)[0]
	if (flg >> 5) != 0:
		return None
	fname = None
	comment = None
	fl.seek(6, 1)
	if (flg & 4) != 0:
		sz = int.from_bytes(fl.read(2), byteorder='little')
		fl.seek(sz, 1)
	if (flg & 8) != 0:
		fname = read_string(fl)
	if (flg & 16) != 0:
		comment = read_string(fl)
	if (flg & 2) != 0:
		fl.seek(2, 1)
	s = fl.tell()
	fl.seek(-8,2)
	szf = fl.tell() - s
	crc = fl.read(4)
	fsize = int.from_bytes(fl.read(4), byteorder='little')
	fl.seek(s, 0)
	return [{'fname':fname, 'comment':comment, 'crc32':crc, 'fsize':fsize}, szf]
