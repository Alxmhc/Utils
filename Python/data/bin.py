def revBit8(c):
	c = ((c&0xaa)>>1) | ((c&0x55)<<1)
	c = ((c&0xcc)>>2) | ((c&0x33)<<2)
	c = (c>>4) | ((c&0x0f)<<4)
	return c
