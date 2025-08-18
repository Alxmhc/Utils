//psapi.lib

#include <vector>
#include <string>

#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>

#undef PROCESSENTRY32
#undef Process32First
#undef Process32Next

#undef MODULEENTRY32
#undef Module32First
#undef Module32Next

DWORD GetModuleFileName_(HMODULE hmd, LPSTR filename, DWORD sz)
{
	return GetModuleFileNameA(hmd, filename, sz);
}
DWORD GetModuleFileName_(HMODULE hmd, LPWSTR filename, DWORD sz)
{
	return GetModuleFileNameW(hmd, filename, sz);
}

DWORD GetModuleFileNameEx_(HANDLE hnd, HMODULE hmd, LPSTR filename, DWORD sz)
{
	return GetModuleFileNameExA(hnd, hmd, filename, sz);
}
DWORD GetModuleFileNameEx_(HANDLE hnd, HMODULE hmd, LPWSTR filename, DWORD sz)
{
	return GetModuleFileNameExW(hnd, hmd, filename, sz);
}

//Program path
template<typename C>
std::basic_string<C> get_cur_pr_path()
{
	C path[MAX_PATH];
	GetModuleFileName_(nullptr, path, MAX_PATH);
	return std::basic_string<C>(path);
}

template<typename C>
std::basic_string<C> get_pr_path(DWORD id)
{
	HANDLE Handle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, id);
	if(!Handle)
		return std::basic_string<C>();
	C path[MAX_PATH];
	GetModuleFileNameEx_(Handle, 0, path, MAX_PATH);
	CloseHandle(Handle);
	return std::basic_string<C>(path);
}

std::vector<PROCESSENTRY32> processList()
{
	std::vector<PROCESSENTRY32> res;
	HANDLE sns = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (sns == INVALID_HANDLE_VALUE)
		return res;

	PROCESSENTRY32 inf;
	inf.dwSize = sizeof(inf);
	if (Process32First(sns, &inf))
	{
		do {
			res.push_back(inf);
		} while ( Process32Next(sns, &inf) );
	}
	CloseHandle(sns);
	return res;
}

std::string processName(DWORD id)
{
	std::string res;
	HANDLE sns = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (sns == INVALID_HANDLE_VALUE)
		return res;
	PROCESSENTRY32 inf;
	inf.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(sns, &inf))
	{
		do {
			if(inf.th32ProcessID == id)
			{
				res = inf.szExeFile;
				break;
			}
		} while ( Process32Next(sns, &inf) );
	}
	CloseHandle(sns);
	return res;
}

std::vector<MODULEENTRY32> moduleList(DWORD id)
{
	std::vector<MODULEENTRY32> res;
	HANDLE sns = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, id);
	if (sns == INVALID_HANDLE_VALUE)
		return res;

	MODULEENTRY32 inf;
	inf.dwSize = sizeof(MODULEENTRY32);
	if(Module32First(sns, &inf))
	{
		do {
			res.push_back(inf);
		} while ( Module32Next(sns, &inf) );
	}
	CloseHandle(sns);
	return res;
}
