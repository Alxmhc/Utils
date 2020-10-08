import os

def dir_(dir, flt, depth):
	l = [(dir,depth)]
	while(l):
		dir, depth = l.pop()
		try:
			lst = os.listdir(dir)
		except Exception:
			continue
		for t in lst:
			e = dir + "/" + t
			is_dir = os.path.isdir(e)
			if is_dir and depth != 0:
				l.append((e, depth-1))
			if flt(e, is_dir):
				yield e

def dir_files(dir, flt, depth = -1):
	def fltr(pth, isDir): return (not isDir) and flt(pth)
	return dir_(dir, fltr, depth)

def dir_folders(dir, flt, depth = -1):
	def fltr(pth, isDir): return isDir and flt(pth)
	return dir_(dir, fltr, depth)


def fls_json(dir, f, d = -1):
	res = {'Name': dir}
	_fls_json(dir, f, d, res)
	return res

def _fls_json(dir, f, d, obj):
	obj['Values'] = []
	obj['Childs'] = []
	try:
		lst = os.listdir(dir)
	except Exception:
		return
	for name in lst:
		e = dir + "/" + name
		if os.path.isfile(e):
			if f(name):
				obj['Values'].append(name)
		elif d != 0:
			tmp = {'Name': name}
			_fls_json(e, f, d - 1, tmp)
			if (len(tmp['Values']) != 0) or (len(tmp['Childs']) != 0):
				obj['Childs'].append(tmp)
