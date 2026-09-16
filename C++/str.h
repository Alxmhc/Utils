#ifndef H_STR
#define H_STR

#include <cstdint>
#include <cstring>
#include <string>

static const uint8_t* bytes(const char* s)
{
	return reinterpret_cast<const uint8_t*>(s);
}

static uint8_t* bytes(char* s)
{
	return reinterpret_cast<uint8_t*>(s);
}

static bool is_b(std::string_view s, std::string_view t)
{
	if(s.size() < t.size())
		return false;
	return std::memcmp(s.data(), t.data(), t.size()) == 0;
}
static bool is_e(std::string_view s, std::string_view t)
{
	if(s.size() < t.size())
		return false;
	return std::memcmp(s.data() + (s.size() - t.size()), t.data(), t.size()) == 0;
}

static void str_lower(std::string &s)
{
	for(std::size_t i = 0; i < s.size(); i++)
	{
		s[i] = static_cast<char>(tolower(s[i]));
	}
}

static void str_upper(std::string &s)
{
	for(std::size_t i = 0; i < s.size(); i++)
	{
		s[i] = static_cast<char>(toupper(s[i]));
	}
}

#endif
