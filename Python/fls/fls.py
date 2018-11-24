import os

def dir_(dir, flt, depth, is_dir):
	l = [(dir,depth)]
	while(l):
		dir, depth = l.pop()
		for t in os.listdir(dir):
			e = dir + "/" + t
			if(os.path.isdir(e)):
				if(is_dir and flt(e)):
					yield e
				if(depth):
					l.append((e, depth-1))
			elif((not is_dir) and flt(e)):
				yield e

def dir_files(dir, flt, depth = -1):
	return dir_(dir, flt, depth, False)

def dir_folders(dir, flt, depth = -1):
	return dir_(dir, flt, depth, True)


def fls_json(dir, f, d = -1):
	res = {'Name': dir}
	_fls_js(dir, f, d, res)
	return res

def _fls_json(dir, f, d, obj):
	obj['Values'] = []
	obj['Childs'] = []
	for name in os.listdir(dir):
		e = dir + "/" + name
		if(os.path.isfile(e)):
			if(f(name)):
				obj['Values'].append(name)
		elif(d):
			tmp = {'Name': name}
			_fls_js(e, f, d - 1, tmp)
			if(len(tmp['Values']) or len(tmp['Childs'])):
				obj['Childs'].append(tmp)
