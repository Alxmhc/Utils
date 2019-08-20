def get_hdr_size(frame):
	if len(frame) < 8:
		return 0
	return 8

def get_src_port(data):
	return int.from_bytes(data[0:2], byteorder='big')
def get_dst_port(data):
	return int.from_bytes(data[2:4], byteorder='big')
