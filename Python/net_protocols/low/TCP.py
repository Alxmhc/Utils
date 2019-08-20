def get_hdr_size(frame):
	l = len(frame)
	if l < 13:
		return 0
	sz = (frame[12]>>4)<<2
	if l < sz or sz < 20:
		return 0
	return sz

def get_src_port(data):
	return int.from_bytes(data[0:2], byteorder='big')
def get_dst_port(data):
	return int.from_bytes(data[2:4], byteorder='big')
def get_num(data):
	return int.from_bytes(data[4:8], byteorder='big')
def get_ack_num(data):
	return int.from_bytes(data[8:12], byteorder='big')
def is_urg(data):
	return (data[13] & 0x20) != 0
def is_ack(data):
	return (data[13] & 0x10) != 0
def is_psh(data):
	return (data[13] & 0x08) != 0
def is_rst(data):
	return (data[13] & 0x04) != 0
def is_syn(data):
	return (data[13] & 0x02) != 0
def is_fin(data):
	return (data[13] & 0x01) != 0
def get_urgent(data):
	return int.from_bytes(data[18:20], byteorder='big')
