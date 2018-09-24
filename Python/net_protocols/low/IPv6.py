def get_hdr_size(frame):
	if len(frame) < 40:
		return 0
	return 40

def get_src(data):
	return data[8:24]
def get_dst(data):
	return data[24:40]
def get_type(data):
	return data[6]
