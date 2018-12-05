#include "lmcons.h"

//Program path
template<typename C>
std::basic_string<C> get_ex_path(void)
{
	C path[MAX_PATH];
    GetModuleFileName_(nullptr, path, MAX_PATH);
	std::basic_string<C> p(path);
	std::replace(p.begin(), p.end(), '\\', '/');
	return p;
}

//Enviroment
template<typename C>
std::vector<std::basic_string<C>> get_env(void)
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
