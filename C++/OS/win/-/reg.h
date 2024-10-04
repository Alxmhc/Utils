#include <string>

#include <windows.h>

namespace reg
{
	HKEY Open(HKEY key, const char* path)
	{
		HKEY res;
		if(RegOpenKeyExA(key, path, 0, KEY_READ, &res) != ERROR_SUCCESS)
			return nullptr;
		return res;
	}

	bool read_inf(HKEY key, const char* name, DWORD &type, DWORD &size)
	{
		return RegQueryValueExA(key, name, nullptr, &type, nullptr, &size) == ERROR_SUCCESS;
	}

	bool Read_String(HKEY key, const char* path, const char* name, std::string &res)
	{
		const auto k = reg::Open(key, path);
		if(k == nullptr)
			return false;
		DWORD type, size;
		if(!read_inf(k, name, type, size) || type != REG_SZ)
		{
			RegCloseKey(k);
			return false;
		}
		res.resize(size);
		const bool e = RegQueryValueExA(k, name, nullptr, nullptr, reinterpret_cast<LPBYTE>(&res[0]), &size) == ERROR_SUCCESS;
		RegCloseKey(k);
		return e;
	}

	bool Read_DWORD(HKEY key, const char* path, const char* name, DWORD &res)
	{
		const auto k = reg::Open(key, path);
		if(k == nullptr)
			return false;
		DWORD type, size;
		if(!read_inf(k, name, type, size) || type != REG_DWORD)
		{
			RegCloseKey(k);
			return false;
		}
		const bool e = RegQueryValueExA(k, name, nullptr, nullptr, reinterpret_cast<LPBYTE>(&res), &size) == ERROR_SUCCESS;
		RegCloseKey(k);
		return e;
	}
};
