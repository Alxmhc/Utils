#include "windows.h"

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

#undef GetEnvironmentStrings
template<typename C> C* GetEnvironmentStrings_(){}
template<> LPCH GetEnvironmentStrings_<char>(){return GetEnvironmentStrings();}
template<> LPWCH GetEnvironmentStrings_<wchar_t>(){return GetEnvironmentStringsW();}

BOOL FreeEnvironmentStrings_(LPCH env){return FreeEnvironmentStringsA(env);}
BOOL FreeEnvironmentStrings_(LPWCH env){return FreeEnvironmentStringsW(env);}

LSTATUS RegOpenKeyEx_(HKEY key, LPCSTR sub, DWORD opt, REGSAM sam, PHKEY res){return RegOpenKeyExA(key, sub, opt, sam, res);}
LSTATUS RegOpenKeyEx_(HKEY key, LPCWSTR sub, DWORD opt, REGSAM sam, PHKEY res){return RegOpenKeyExW(key, sub, opt, sam, res);}

LSTATUS RegQueryValueEx_(HKEY key, LPCSTR name, LPDWORD res, LPDWORD type, LPBYTE data, LPDWORD size){return RegQueryValueExA(key, name, res, type, data, size);}
LSTATUS RegQueryValueEx_(HKEY key, LPCWSTR name, LPDWORD res, LPDWORD type, LPBYTE data, LPDWORD size){return RegQueryValueExW(key, name, res, type, data, size);}

namespace reg
{
	bool read(HKEY key, const std::string &path, const std::string &name, DWORD &val)
	{
		if(RegOpenKeyExA(key, path.c_str(), NULL, KEY_QUERY_VALUE, &key) != ERROR_SUCCESS)
			return false;
		DWORD size = sizeof(DWORD);
		return RegQueryValueExA(key, name.c_str(), NULL, NULL, reinterpret_cast<LPBYTE>(&val), &size) == ERROR_SUCCESS;
	}

	bool read(HKEY key, const std::string &path, const std::string &name, std::string &val)
	{
		if(RegOpenKeyExA(key, path.c_str(), NULL, KEY_QUERY_VALUE, &key) != ERROR_SUCCESS)
			return false;
		DWORD size = 0;
		if(RegQueryValueExA(key, name.c_str(), NULL, NULL, NULL, &size) != ERROR_SUCCESS)
			return false;
		char* r = new char[size];
		RegQueryValueExA(key, name.c_str(), NULL, NULL, reinterpret_cast<LPBYTE>(r), &size);
		val = std::string(r);
		delete[] r;
		return true;
	}
}
