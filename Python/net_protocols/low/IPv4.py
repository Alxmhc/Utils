def get_hdr_size(frame):
	l = len(frame)
	if l < 1:
		return 0
	sz = (frame[0] & 0x0f)<<2
	if l < sz or sz < 20:
		return 0
	return sz

def get_src(data):
	return data[12:16]
def get_dst(data):
	return data[16:20]
def get_id(data):
	return int.from_bytes(data[4:6], byteorder='big')
def get_type(data):
	return data[9]
