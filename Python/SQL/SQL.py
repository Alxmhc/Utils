def select(t, cl, ext=''):
	return 'SELECT '+','.join(cl)+' FROM '+t+ext

def insert(t, vl, cl = None):
	s = 'INSERT INTO '+t
	if(not cl is None):
		s += '('+','.join(cl)+')'
	v = map(lambda e: '"'+e+'"' if (type(e) is str) else str(e), vl)
	return s+' VALUES('+','.join(v)+')'
