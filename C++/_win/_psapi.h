#include "psapi.h"

DWORD GetModuleFileNameEx_(HANDLE hnd, HMODULE hmd, LPWSTR filename, DWORD sz){return GetModuleFileNameExW(hnd, hmd, filename, sz);}
DWORD GetModuleFileNameEx_(HANDLE hnd, HMODULE hmd, LPSTR filename, DWORD sz){return GetModuleFileNameExA(hnd, hmd, filename, sz);}

DWORD GetProcessImageFileName_(HANDLE hnd, LPSTR filename, DWORD sz){return GetProcessImageFileNameA(hnd, filename, sz);}
DWORD GetProcessImageFileName_(HANDLE hnd, LPWSTR filename, DWORD sz){return GetProcessImageFileNameW(hnd, filename, sz);}
