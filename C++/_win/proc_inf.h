#include <tlhelp32.h>

#undef PROCESSENTRY32
#undef Process32First
#undef Process32Next

#undef MODULEENTRY32
#undef Module32First
#undef Module32Next

struct procInf
{
	DWORD id;
	std::string name;

	procInf(DWORD c, const char *n) : id(c), name(n) {}
};

std::vector<procInf> processList()
{
	std::vector<procInf> res;

	HANDLE sns = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if ( sns == INVALID_HANDLE_VALUE )
		return res;

	PROCESSENTRY32 inf;
	inf.dwSize = sizeof(inf);
	if (Process32First(sns, &inf))
	{
		do {
			res.push_back( procInf(inf.th32ProcessID, inf.szExeFile) );
		} while ( Process32Next(sns, &inf) );
	}
	CloseHandle(sns);
	return res;
}

struct moduleInf
{
	std::string path;
	const uint8_t* addr;
	DWORD size;
	DWORD procID;

	moduleInf(DWORD id, const uint8_t *h, DWORD sz, const char *p) : procID(id), addr(h), size(sz), path(p) {}
};

std::vector<moduleInf> moduleList(DWORD id)
{
	std::vector<moduleInf> res;
	HANDLE sns = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, id);
	if (sns == INVALID_HANDLE_VALUE)
		return res;

	MODULEENTRY32 inf;
	inf.dwSize = sizeof(MODULEENTRY32);
	if(Module32First(sns, &inf))
	{
		do {
			res.push_back( moduleInf(id, inf.modBaseAddr, inf.modBaseSize, inf.szExePath) );
		} while ( Module32Next(sns, &inf) );
	}
	CloseHandle(sns);
	return res;
}
