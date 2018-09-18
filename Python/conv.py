def list_dict(l, d):
	n = len(d)
	r = {}
	for st in l:
		p = st.find(d)
		if(p == -1):
			continue
		r[st[:p]] = st[p+n:]
	return r
