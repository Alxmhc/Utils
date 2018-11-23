def bytes_int_be(b):
	r = 0
	for i in range(len(b)):
		r = (r<<8) + b[i]
	return r

def bytes_int_le(b):
	r = 0
	for i in reversed(range(len(b))):
		r = (r<<8) + b[i]
	return r
