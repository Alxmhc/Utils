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
	return std::memcmp(s.c_str(), t.c_str(), t.length()) == 0;
}
static bool is_e(const std::string &s, const std::string &t)
{
	if(s.length() < t.length())
		return false;
	return std::memcmp(s.c_str() + (s.length() - t.length()), t.c_str(), t.length()) == 0;
}

static void str_lower(std::string &s)
{
	for(std::size_t i = 0; i < s.size(); i++)
	{
		s[i] = tolower(s[i]);
	}
}

#endif
