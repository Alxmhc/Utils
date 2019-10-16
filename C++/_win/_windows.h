#include "windows.h"

#undef GetEnvironmentStrings

DWORD GetModuleFileName_(HMODULE hmd, LPSTR filename, DWORD sz){return GetModuleFileNameA(hmd, filename, sz);}
DWORD GetModuleFileName_(HMODULE hmd, LPWSTR filename, DWORD sz){return GetModuleFileNameW(hmd, filename, sz);}

template<typename S> struct WIN32_FIND_DATA_{};
template<> struct WIN32_FIND_DATA_<char>{typedef WIN32_FIND_DATAA T;};
template<> struct WIN32_FIND_DATA_<std::string>{typedef WIN32_FIND_DATAA T;};
template<> struct WIN32_FIND_DATA_<wchar_t>{typedef WIN32_FIND_DATAW T;};
template<> struct WIN32_FIND_DATA_<std::wstring>{typedef WIN32_FIND_DATAW T;};

HANDLE FindFirstFile_(LPCSTR filename, LPWIN32_FIND_DATAA ffd){return FindFirstFileA(filename, ffd);}
HANDLE FindFirstFile_(LPCWSTR filename, LPWIN32_FIND_DATAW ffd){return FindFirstFileW(filename, ffd);}

BOOL FindNextFile_(HANDLE hff, LPWIN32_FIND_DATAA ffd){return FindNextFileA(hff, ffd);}
BOOL FindNextFile_(HANDLE hff, LPWIN32_FIND_DATAW ffd){return FindNextFileW(hff, ffd);}

BOOL CreateDirectory_(LPCSTR path, LPSECURITY_ATTRIBUTES attr){return CreateDirectoryA(path, attr);}
BOOL CreateDirectory_(LPCWSTR path, LPSECURITY_ATTRIBUTES attr){return CreateDirectoryW(path, attr);}

BOOL MoveFileEx_(LPCSTR old_path, LPCSTR new_path, DWORD flags){return MoveFileExA(old_path, new_path, flags);}
BOOL MoveFileEx_(LPCWSTR old_path, LPCWSTR new_path, DWORD flags){return MoveFileExW(old_path, new_path, flags);}

template<typename C> C* GetEnvironmentStrings_(){}
template<> LPCH GetEnvironmentStrings_<char>(){return GetEnvironmentStrings();}
template<> LPWCH GetEnvironmentStrings_<wchar_t>(){return GetEnvironmentStringsW();}

BOOL FreeEnvironmentStrings_(LPCH env){return FreeEnvironmentStringsA(env);}
BOOL FreeEnvironmentStrings_(LPWCH env){return FreeEnvironmentStringsW(env);}

HMODULE LoadLibrary_(LPCSTR filename){return LoadLibraryA(filename);}
HMODULE LoadLibrary_(LPCWSTR filename){return LoadLibraryW(filename);}

BOOL GetUserName_(LPSTR buf, LPDWORD sz){return GetUserNameA(buf, sz);}
BOOL GetUserName_(LPWSTR buf, LPDWORD sz){return GetUserNameW(buf, sz);}

BOOL GetComputerName_(LPSTR buf, LPDWORD sz){return GetComputerNameA(buf, sz);}
BOOL GetComputerName_(LPWSTR buf, LPDWORD sz){return GetComputerNameW(buf, sz);}
