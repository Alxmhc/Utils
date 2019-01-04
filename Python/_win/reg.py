import winreg

def read(hkey, path, name):
	k = winreg.OpenKey(getattr(winreg,hkey), path)
	res = winreg.QueryValueEx(k, name)
	winreg.CloseKey(k)
	return res[0]
