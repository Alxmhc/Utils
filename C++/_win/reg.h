LSTATUS RegOpenKeyEx_(HKEY key, LPCSTR sub, DWORD opt, REGSAM sam, PHKEY res){return RegOpenKeyExA(key, sub, opt, sam, res);}
LSTATUS RegOpenKeyEx_(HKEY key, LPCWSTR sub, DWORD opt, REGSAM sam, PHKEY res){return RegOpenKeyExW(key, sub, opt, sam, res);}

LSTATUS RegQueryValueEx_(HKEY key, LPCSTR name, LPDWORD res, LPDWORD type, LPBYTE data, LPDWORD size){return RegQueryValueExA(key, name, res, type, data, size);}
LSTATUS RegQueryValueEx_(HKEY key, LPCWSTR name, LPDWORD res, LPDWORD type, LPBYTE data, LPDWORD size){return RegQueryValueExW(key, name, res, type, data, size);}

namespace reg
{
	bool read(HKEY key, const std::string &path, const std::string &name, DWORD &val)
	{
		if(RegOpenKeyExA(key, path.c_str(), 0, KEY_QUERY_VALUE, &key) != ERROR_SUCCESS)
			return false;
		DWORD size = sizeof(DWORD);
		return RegQueryValueExA(key, name.c_str(), nullptr, nullptr, reinterpret_cast<LPBYTE>(&val), &size) == ERROR_SUCCESS;
	}

	bool read(HKEY key, const std::string &path, const std::string &name, std::string &val)
	{
		if(RegOpenKeyExA(key, path.c_str(), 0, KEY_QUERY_VALUE, &key) != ERROR_SUCCESS)
			return false;
		DWORD size = 0;
		if(RegQueryValueExA(key, name.c_str(), nullptr, nullptr, nullptr, &size) != ERROR_SUCCESS)
			return false;
		char* r = new char[size];
		RegQueryValueExA(key, name.c_str(), nullptr, nullptr, reinterpret_cast<LPBYTE>(r), &size);
		val = std::string(r);
		delete[] r;
		return true;
	}
}
