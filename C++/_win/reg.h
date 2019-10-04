LSTATUS RegOpenKeyEx_(HKEY key, LPCSTR sub, DWORD opt, REGSAM sam, PHKEY res){return RegOpenKeyExA(key, sub, opt, sam, res);}
LSTATUS RegOpenKeyEx_(HKEY key, LPCWSTR sub, DWORD opt, REGSAM sam, PHKEY res){return RegOpenKeyExW(key, sub, opt, sam, res);}

LSTATUS RegQueryValueEx_(HKEY key, LPCSTR name, LPDWORD res, LPDWORD type, LPBYTE data, LPDWORD size){return RegQueryValueExA(key, name, res, type, data, size);}
LSTATUS RegQueryValueEx_(HKEY key, LPCWSTR name, LPDWORD res, LPDWORD type, LPBYTE data, LPDWORD size){return RegQueryValueExW(key, name, res, type, data, size);}

namespace reg
{
	bool read(HKEY key, const char *path, const char *name, DWORD &val)
	{
		if(RegOpenKeyExA(key, path, 0, KEY_QUERY_VALUE, &key) != ERROR_SUCCESS)
			return false;
		DWORD size = sizeof(DWORD);
		auto t = RegQueryValueExA(key, name, nullptr, nullptr, reinterpret_cast<LPBYTE>(&val), &size);
		return t == ERROR_SUCCESS;
	}

	bool read(HKEY key, const char *path, const char *name, std::string &val)
	{
		if(RegOpenKeyExA(key, path, 0, KEY_QUERY_VALUE, &key) != ERROR_SUCCESS)
			return false;
		DWORD size = 0;
		if(RegQueryValueExA(key, name, nullptr, nullptr, nullptr, &size) != ERROR_SUCCESS)
			return false;
		val.resize(size);
		auto t = RegQueryValueExA(key, name, nullptr, nullptr, reinterpret_cast<LPBYTE>(&val[0]), &size);
		if(t != ERROR_SUCCESS)
			return false;
		val.pop_back();
		return true;
	}
}
