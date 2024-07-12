#ifndef H_B
#define H_B

#include <cstdint>

static const uint8_t* bytes(const char* s)
{
	return reinterpret_cast<const uint8_t*>(s);
}

static uint8_t* bytes(char* s)
{
	return reinterpret_cast<uint8_t*>(s);
}

#endif
