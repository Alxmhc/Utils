//psapi.lib

#include <vector>
#include <string>
#include <filesystem>

#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>

std::filesystem::path get_pr_path(DWORD id)
{
	std::filesystem::path res;
	HANDLE Handle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, id);
	if (!Handle)
		return res;
	TCHAR path[MAX_PATH];
	if (GetModuleFileNameEx(Handle, 0, path, MAX_PATH) != 0)
	{
		res = path;
	}
	CloseHandle(Handle);
	return res;
}

template<class F>
void pr_process(F &fnc)
{
	HANDLE sns = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (sns == INVALID_HANDLE_VALUE)
		return;
	PROCESSENTRY32 inf;
	inf.dwSize = sizeof(inf);
	if (Process32First(sns, &inf))
	{
		do {
			if (fnc(inf))
				break;
		} while ( Process32Next(sns, &inf) );
	}
	CloseHandle(sns);
}

template<class F>
void pr_module(F &fnc, DWORD id)
{
	HANDLE sns = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, id);
	if (sns == INVALID_HANDLE_VALUE)
		return;
	MODULEENTRY32 inf;
	inf.dwSize = sizeof(inf);
	if(Module32First(sns, &inf))
	{
		do {
			if (fnc(inf))
				break;
		} while ( Module32Next(sns, &inf) );
	}
	CloseHandle(sns);
}
