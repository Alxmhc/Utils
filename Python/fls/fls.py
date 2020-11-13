import os

def dir_(dir, depth):
	l = [(dir,depth)]
	while(l):
		dir, depth = l.pop()
		try:
			lst = os.listdir(dir)
		except Exception:
			continue
		for t in lst:
			e = os.path.join(dir, t)
			is_dir = os.path.isdir(e)
			if is_dir and depth != 0:
				l.append((e, depth-1))
			yield e, is_dir

def dir_files(dir, depth = -1):
	return (p for p,f in dir_(dir, depth) if not f)

def dir_folders(dir, depth = -1):
	return (p for p,f in dir_(dir, depth) if f)


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
		e = os.path.join(dir, name)
		if os.path.isfile(e):
			if f(name):
				obj['Values'].append(name)
		elif d != 0:
			tmp = {'Name': name}
			_fls_json(e, f, d - 1, tmp)
			if (len(tmp['Values']) != 0) or (len(tmp['Childs']) != 0):
				obj['Childs'].append(tmp)
