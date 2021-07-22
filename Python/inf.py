def list_dict(l, d):
	n = len(d)
	r = {}
	for st in l:
		p = st.find(d)
		if(p != -1):
			r[st[:p]] = st[p+n:]
	return r
