def list_dict(l, d):
	n = len(d)
	r = {}
	for st in l:
		p = st.find(d)
		if(p != -1):
			r[st[:p]] = st[p+n:]
	return r

def is_pal(m):
	l = len(m)
	for i in range(l>>1):
		if(m[i] != m[l-i-1]):
			return False
	return True
