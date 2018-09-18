def fls(dir, f, d = -1):
	l = [(dir,d)]
	while(l):
		dir, d = l.pop()
		for t in os.listdir(dir):
			e = dir + "/" + t
			if(os.path.isfile(e)):
				if(f(t)):
					yield e
			elif(d):
				l.append((e, d-1))

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
