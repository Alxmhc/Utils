def is_pal(m):
	l = len(m)
	for i in range(l>>1):
		if(m[i] != m[l-i-1]):
			return False
	return True
