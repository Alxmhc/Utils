from fls.parse import jsonlz4
from data import lz4

def read(fl):
	hdr = fl.read(8)
	if(hdr != b"\x6d\x6f\x7a\x4c\x7a\x34\x30\x00"):
		return 0
	sz = fl.read(4)
	return int.from_bytes(sz, byteorder='little')

def unpack(f_in, f_out):
	with open(f_in, "rb") as fin:
		sz = jsonlz4.read(fin)
		if(sz == 0):
			return
		data = lz4.decode_block(fin)
		fout = open(f_out, "wb")
		fout.write(data)
		fout.close()
