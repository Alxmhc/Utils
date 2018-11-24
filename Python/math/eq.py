#a[0]*x+a[1]*y+a[2]=0; b[0]*x+b[1]*y+b[2]=0
def eq1_2(a,b):
	t = v3_pr(a,b)
	return [t[0]/t[2], t[1]/t[2]]
