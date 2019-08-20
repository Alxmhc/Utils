def get_hdr_size(frame):
	if len(frame)<14:
		return 0
	return 14

def get_src(data):
	return data[6:12]
def get_dst(data):
	return data[:6]
def get_type(data):
	return int.from_bytes(data[12:14], byteorder='big')
