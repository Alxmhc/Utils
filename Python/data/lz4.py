from data import decompr

def _get_size(b, sz):
	if sz != 15:
		return sz
	while True:
		k = ord( b.read(1) )
		sz += k
		if k != 255:
			break
	return sz

def decode_block(b):
	res = bytearray()
	while True:
		f = ord( b.read(1) )
		size = _get_size(b, f >> 4)
		if size != 0:
			res += b.read(size)
		s = b.read(2)
		if s == b'':
			break
		offset = int.from_bytes(s, byteorder='little')
		len = _get_size(b, f & 0xf) + 4
		decompr.LZ77_repeat(len, offset, res)
	return res
