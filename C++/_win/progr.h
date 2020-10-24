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

template<typename C>
std::basic_string<C> get_process_path(DWORD PID)
{
	HANDLE Handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, PID);
	if(!Handle)
		return std::basic_string<C>();
	C path[MAX_PATH];
	DWORD l = GetModuleFileNameEx_(Handle, 0, path, MAX_PATH);
	CloseHandle(Handle);
	if (l == 0)
		return std::basic_string<C>();
	std::basic_string<C> r(path);
	std::replace(r.begin(), r.end(), '\\', '/');
	return r;
}

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
