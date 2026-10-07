//psapi.lib

#include <filesystem>

#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>

std::filesystem::path get_pr_path(DWORD id)
{
	std::filesystem::path res;
	HANDLE Handle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, id);
	if (!Handle)
		return res;
	TCHAR path[MAX_PATH];
	DWORD sz = MAX_PATH;
	if (QueryFullProcessImageName(Handle, 0, path, &sz) == TRUE)
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

bool terminate_ID(DWORD id, UINT ec = 0)
{
	HANDLE Handle = OpenProcess(PROCESS_TERMINATE, FALSE, id);
	if (!Handle)
		return false;
	const bool res = TerminateProcess(Handle, ec) == TRUE;
	CloseHandle(Handle);
	return res;
}
