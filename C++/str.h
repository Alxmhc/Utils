#ifndef H_STR
#define H_STR

#include <cstdint>
#include <string>

static const uint8_t* bytes(const char* s)
{
	return reinterpret_cast<const uint8_t*>(s);
}

static uint8_t* bytes(char* s)
{
	return reinterpret_cast<uint8_t*>(s);
}

static bool is_b(const std::string &s, const std::string &t)
{
	if(s.length() < t.length())
		return false;
	return s.substr(0, t.length()) == t;
}
static bool is_e(const std::string &s, const std::string &t)
{
	if(s.length() < t.length())
		return false;
	return s.substr(s.length() - t.length()) == t;
}

#endif
