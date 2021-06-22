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
bool Account_Sid(const PSID SID, std::basic_string<C> &Name, std::basic_string<C> &Domain, PSID_NAME_USE u, const C *sysName = nullptr)
{
	DWORD szn = 0, szd = 0;
	LookupAccountSid_(sysName, SID, nullptr, &szn, nullptr, &szd, u);
	if(szn == 0 || szd == 0)
		return false;
	Name.resize(szn);
	Domain.resize(szd);
	if( LookupAccountSid_(sysName, SID, &Name[0], &szn, &Domain[0], &szd, u) != TRUE )
		return false;
	Name.pop_back();
	Domain.pop_back();
	return true;
}
