#include <vector>
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

	bool Read_String(HKEY key, const char* path, const char* name, std::string &res)
	{
		const auto k = reg::Open(key, path);
		if(k == nullptr)
			return false;
		DWORD type, size;
		bool r = false;
		if(RegQueryValueExA(k, name, nullptr, &type, nullptr, &size) == ERROR_SUCCESS
		&& type == REG_SZ)
		{
			res.resize(size);
			r = RegQueryValueExA(k, name, nullptr, nullptr, reinterpret_cast<LPBYTE>(&res[0]), &size) == ERROR_SUCCESS;
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
		if(RegQueryValueExA(k, name, nullptr, &type, nullptr, &size) == ERROR_SUCCESS
		&& type == REG_DWORD)
		{
			r = RegQueryValueExA(k, name, nullptr, nullptr, reinterpret_cast<LPBYTE>(&res), &size) == ERROR_SUCCESS;
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
		if(RegQueryValueExA(k, name, nullptr, &type, nullptr, &size) == ERROR_SUCCESS
		&& type == REG_BINARY)
		{
			res.resize(size);
			r = RegQueryValueExA(k, name, nullptr, nullptr, reinterpret_cast<LPBYTE>(res.data()), &size) == ERROR_SUCCESS;
		}
		RegCloseKey(k);
		return r;
	}
};
