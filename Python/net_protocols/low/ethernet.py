def get_hdr_size(frame):
	if len(frame)<14:
		return 0
	return 14

def get_type(data):
	return (data[12]<<8) + data[13]
