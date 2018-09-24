def get_hdr_size(frame):
	l = len(frame)
	if l < 13:
		return 0
	sz = (frame[12]>>4)<<2
	if l < sz or sz < 20:
		return 0
	return sz

def get_src_port(data):
	return (data[0]<<8) + data[1]
def get_dst_port(data):
	return (data[2]<<8) + data[3]
def get_num(data):
	return (data[4]<<24) + (data[5]<<16) + (data[6]<<8) + data[7]
def get_ack_num(data):
	return (data[8]<<24) + (data[9]<<16) + (data[10]<<8) + data[11]
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
	return (data[18]<<8) + data[19]
