def get_hdr_size(frame):
	if len(frame) < 8:
		return 0
	return 8

def get_src_port(data):
	return (data[0]<<8) + data[1]
def get_dst_port(data):
	return (data[2]<<8) + data[3]
