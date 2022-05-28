//psapi.lib
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
