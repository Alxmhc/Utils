const uint8_t* bytes(const char* s)
{
	return reinterpret_cast<const uint8_t*>(s);
}

uint8_t* bytes(char* s)
{
	return reinterpret_cast<uint8_t*>(s);
}
