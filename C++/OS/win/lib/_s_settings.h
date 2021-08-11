#include <windows.h>
#include <lmcons.h>

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

LSTATUS RegOpenKeyEx_(HKEY key, LPCSTR sub, DWORD opt, REGSAM sam, PHKEY res)
{
	return RegOpenKeyExA(key, sub, opt, sam, res);
}
LSTATUS RegOpenKeyEx_(HKEY key, LPCWSTR sub, DWORD opt, REGSAM sam, PHKEY res)
{
	return RegOpenKeyExW(key, sub, opt, sam, res);
}

LSTATUS RegQueryValueEx_(HKEY key, LPCSTR name, LPDWORD res, LPDWORD type, LPBYTE data, LPDWORD size)
{
	return RegQueryValueExA(key, name, res, type, data, size);
}
LSTATUS RegQueryValueEx_(HKEY key, LPCWSTR name, LPDWORD res, LPDWORD type, LPBYTE data, LPDWORD size)
{
	return RegQueryValueExW(key, name, res, type, data, size);
}

BOOL LookupAccountSid_(LPCSTR sysName, PSID SID, LPSTR Name, LPDWORD NameSz, LPSTR Domain, LPDWORD DomainSz, PSID_NAME_USE u)
{
	return LookupAccountSidA(sysName, SID, Name, NameSz, Domain, DomainSz, u);
}
BOOL LookupAccountSid_(LPCWSTR sysName, PSID SID, LPWSTR Name, LPDWORD NameSz, LPWSTR Domain, LPDWORD DomainSz, PSID_NAME_USE u)
{
	return LookupAccountSidW(sysName, SID, Name, NameSz, Domain, DomainSz, u);
}
