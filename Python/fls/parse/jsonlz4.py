import json
from data import lz4

def read_inf(fl):
	hdr = fl.read(8)
	if(hdr != b"\x6d\x6f\x7a\x4c\x7a\x34\x30\x00"):
		return 0
	sz = fl.read(4)
	return int.from_bytes(sz, byteorder='little')

def read(fl):
	with open(fl, "rb") as fin:
		sz = read_inf(fin)
		if(sz == 0):
			return None
		return lz4.decode_block(fin)

def parse(fl):
	data = read(fl)
	if data == None:
		return None
	return json.loads(data)
