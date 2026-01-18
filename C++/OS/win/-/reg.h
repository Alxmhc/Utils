#include <cstdint>
#include <vector>
#include <string>

#include <windows.h>

LSTATUS RegOpenKeyEx_(HKEY key, LPCSTR pth, REGSAM acc, PHKEY res)
{
	return RegOpenKeyExA(key, pth, 0, acc, res);
}
LSTATUS RegOpenKeyEx_(HKEY key, LPCWSTR pth, DWORD opt, REGSAM acc, PHKEY res)
{
	return RegOpenKeyExW(key, pth, 0, acc, res);
}

LSTATUS RegQueryValueEx_(HKEY key, LPCSTR name, LPDWORD type, LPBYTE d, LPDWORD sz)
{
	return RegQueryValueExA(key, name, nullptr, type, d, sz);
}
LSTATUS RegQueryValueEx_(HKEY key, LPCWSTR name, LPDWORD type, LPBYTE d, LPDWORD sz)
{
	return RegQueryValueExW(key, name, nullptr, type, d, sz);
}

namespace reg
{
	HKEY Open(HKEY key, const char* path)
	{
		HKEY res;
		if(RegOpenKeyEx_(key, path, KEY_READ, &res) != ERROR_SUCCESS)
			return nullptr;
		return res;
	}

	bool Read_String(HKEY key, const char* path, const char* name, std::string &res)
	{
		const auto k = reg::Open(key, path);
		if(k == nullptr)
			return false;
		DWORD type, size;
		bool r = false;
		if(RegQueryValueEx_(k, name, &type, nullptr, &size) == ERROR_SUCCESS
		&& type == REG_SZ)
		{
			res.resize(size);
			r = RegQueryValueEx_(k, name, nullptr, reinterpret_cast<LPBYTE>(&res[0]), &size) == ERROR_SUCCESS;
		}
		RegCloseKey(k);
		return r;
	}

	bool Read_DWORD(HKEY key, const char* path, const char* name, DWORD &res)
	{
		const auto k = reg::Open(key, path);
		if(k == nullptr)
			return false;
		DWORD type, size;
		bool r = false;
		if(RegQueryValueEx_(k, name, &type, nullptr, &size) == ERROR_SUCCESS
		&& type == REG_DWORD)
		{
			r = RegQueryValueEx_(k, name, nullptr, reinterpret_cast<LPBYTE>(&res), &size) == ERROR_SUCCESS;
		}
		RegCloseKey(k);
		return r;
	}

	bool Read_BIN(HKEY key, const char* path, const char* name, std::vector<uint8_t> &res)
	{
		const auto k = reg::Open(key, path);
		if(k == nullptr)
			return false;
		DWORD type, size;
		bool r = false;
		if(RegQueryValueEx_(k, name, &type, nullptr, &size) == ERROR_SUCCESS
		&& type == REG_BINARY)
		{
			res.resize(size);
			r = RegQueryValueEx_(k, name, nullptr, reinterpret_cast<LPBYTE>(res.data()), &size) == ERROR_SUCCESS;
		}
		RegCloseKey(k);
		return r;
	}
};
