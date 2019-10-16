#include <tlhelp32.h>

#undef PROCESSENTRY32
#undef Process32First
#undef Process32Next

struct procInf {
	DWORD id;
	std::string name;
};

std::vector<procInf> processList()
{
	std::vector<procInf> res;

	HANDLE sns = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if ( sns == INVALID_HANDLE_VALUE )
		return res;

	PROCESSENTRY32 inf;
	inf.dwSize = sizeof(inf);
	Process32First(sns, &inf);
	for(;;)
	{
		procInf tmp = {inf.th32ProcessID, inf.szExeFile};
		res.push_back(tmp);
		if( !Process32Next(sns, &inf) )
			break;
	}
	CloseHandle(sns);
	return res;
}
