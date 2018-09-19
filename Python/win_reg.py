def reg_read(hkey, path, name):
	k = _winreg.OpenKey(getattr(_winreg,hkey), path)
	res = _winreg.QueryValueEx(k, name)
	_winreg.CloseKey(k)
	return res[0]
