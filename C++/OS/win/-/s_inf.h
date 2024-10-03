//Secur32.lib

#include <vector>
#include <string>

#include <windows.h>
#include <lmcons.h>

#define SECURITY_WIN32
#include <Security.h>

BOOL GetUserName_(LPSTR buf, LPDWORD sz)
{
	return GetUserNameA(buf, sz);
}
BOOL GetUserName_(LPWSTR buf, LPDWORD sz)
{
	return GetUserNameW(buf, sz);
}

BOOL GetComputerName_(LPSTR buf, LPDWORD sz)
{
	return GetComputerNameA(buf, sz);
}
BOOL GetComputerName_(LPWSTR buf, LPDWORD sz)
{
	return GetComputerNameW(buf, sz);
}

BOOL GetUserNameEx_(EXTENDED_NAME_FORMAT format, LPSTR buf, LPDWORD sz)
{
	return GetUserNameExA(format, buf, sz);
}
BOOL GetUserNameEx_(EXTENDED_NAME_FORMAT format, LPWSTR buf, LPDWORD sz)
{
	return GetUserNameExW(format, buf, sz);
}

template<typename C>
std::basic_string<C> get_username()
{
	C username[UNLEN + 1];
	DWORD sz = UNLEN + 1;
	GetUserName_(username, &sz);
	std::basic_string<C> r(username);
	return r;
}

template<typename C>
std::basic_string<C> get_computername()
{
	C computername[CNLEN + 1];
	DWORD sz = CNLEN + 1;
	GetComputerName_(computername, &sz);
	std::basic_string<C> r(computername);
	return r;
}

template<typename C>
std::basic_string<C> get_username_f()
{
	C username[UNLEN + 1];
	DWORD sz = UNLEN + 1;
	GetUserNameEx_(NameSamCompatible, username, &sz);
	std::basic_string<C> r(username);
	return r;
}

#undef GetEnvironmentStrings
template<typename C> C* GetEnvironmentStrings_(){}
template<> LPCH GetEnvironmentStrings_<char>()
{
	return GetEnvironmentStrings();
}
template<> LPWCH GetEnvironmentStrings_<wchar_t>()
{
	return GetEnvironmentStringsW();
}

BOOL FreeEnvironmentStrings_(LPCH env)
{
	return FreeEnvironmentStringsA(env);
}
BOOL FreeEnvironmentStrings_(LPWCH env)
{
	return FreeEnvironmentStringsW(env);
}

//Enviroment
template<typename C>
std::vector<std::basic_string<C>> get_env()
{
	std::vector<std::basic_string<C>> res;
	auto env = GetEnvironmentStrings_<C>();
	auto s = env;
	while(*s != 0)
	{
		std::basic_string<C> str(s);
		res.push_back(str);
		s += str.length();
		s++;
	}
	FreeEnvironmentStrings_(env);
	return res;
}
