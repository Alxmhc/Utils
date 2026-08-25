#include <vector>
#include <string>
#include <algorithm>

#include <windows.h>

bool RegOpenKeyEx_(HKEY key, LPCSTR pth, REGSAM acc, PHKEY res)
{
	return RegOpenKeyExA(key, pth, 0, acc, res) == ERROR_SUCCESS;
}
bool RegOpenKeyEx_(HKEY key, LPCWSTR pth, REGSAM acc, PHKEY res)
{
	return RegOpenKeyExW(key, pth, 0, acc, res) == ERROR_SUCCESS;
}

bool RegQueryValueEx_(HKEY key, LPCSTR name, LPDWORD type, LPBYTE d, LPDWORD sz)
{
	return RegQueryValueExA(key, name, nullptr, type, d, sz) == ERROR_SUCCESS;
}
bool RegQueryValueEx_(HKEY key, LPCWSTR name, LPDWORD type, LPBYTE d, LPDWORD sz)
{
	return RegQueryValueExW(key, name, nullptr, type, d, sz) == ERROR_SUCCESS;
}

namespace reg
{
	HKEY Open(HKEY key, std::string path)
	{
		std::replace(path.begin(), path.end(), '/', '\\');
		HKEY res;
		if(!RegOpenKeyEx_(key, path.c_str(), KEY_READ, &res))
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
		if(RegQueryValueEx_(k, name, &type, nullptr, &size)
		&& type == REG_SZ)
		{
			res.resize(size);
			r = RegQueryValueEx_(k, name, nullptr, reinterpret_cast<LPBYTE>(&res[0]), &size);
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
		if(RegQueryValueEx_(k, name, &type, nullptr, &size)
		&& type == REG_DWORD)
		{
			r = RegQueryValueEx_(k, name, nullptr, reinterpret_cast<LPBYTE>(&res), &size);
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
		if(RegQueryValueEx_(k, name, &type, nullptr, &size)
		&& type == REG_BINARY)
		{
			res.resize(size);
			r = RegQueryValueEx_(k, name, nullptr, reinterpret_cast<LPBYTE>(res.data()), &size);
		}
		RegCloseKey(k);
		return r;
	}
};
