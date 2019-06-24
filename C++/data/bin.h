uint8_t bitRev(uint8_t c)
{
	c = ((c & 0xAA)>>1)|((c & 0x55)<<1);
	c = ((c & 0xCC)>>2)|((c & 0x33)<<2);
	c = (c>>4)|(c<<4);
	return c;
}

uint16_t bitRev(uint16_t c)
{
	c = ((c & 0xAAAA)>>1)|((c & 0x5555)<<1);
	c = ((c & 0xCCCC)>>2)|((c & 0x3333)<<2);
	c = ((c & 0xF0F0)>>4)|((c & 0x0F0F)<<4);
	c = (c>>8)|(c<<8);
	return c;
}

uint32_t bitRev(uint32_t c)
{
	c = ((c & 0xAAAAAAAA)>>1)|((c & 0x55555555)<<1);
	c = ((c & 0xCCCCCCCC)>>2)|((c & 0x33333333)<<2);
	c = ((c & 0xF0F0F0F0)>>4)|((c & 0x0F0F0F0F)<<4);
	c = ((c & 0xFF00FF00)>>8)|((c & 0x00FF00FF)<<8);
	c = (c>>16)|(c<<16);
	return c;
}
