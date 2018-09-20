def v3_pr(a,b):
	return [a[1]*b[2]-a[2]*b[1],a[2]*b[0]-a[0]*b[2],a[0]*b[1]-a[1]*b[0]]

#a[0]*x+a[1]*y+a[2]=0; b[0]*x+b[1]*y+b[2]=0
def eq1_2(a,b):
	t = v3_pr(a,b)
	return [t[0]/t[2], t[1]/t[2]]

#y=a+b*x
def linappr(x,y):
	n = len(x)
	a = [float(n),0.0,0.0]
	b = [0.0,0.0,0.0]
	for i in range(n):
		a[1] += x[i]
		b[1] += x[i] * x[i]
		a[2] -= y[i]
		b[2] -= x[i] * y[i]
	b[0] = a[1]
	return eq1_2(a,b)

#y=a+b/x
def gipappr(x,y):
	n = len(x)
	a = [float(n),0.0,0.0]
	b = [0.0,0.0,0.0]
	for i in range(n):
		k = 1.0 / x[i]
		a[1] += k
		b[1] += k * k
		a[2] -= y[i]
		b[2] -= k * y[i]
	b[0] = a[1]
	return eq1_2(a,b)

#f(x)=0; a<=x<=b
def solve_eq(a,b,f,d):
	x = f(a)
	if(x>-d and x<d):
		return a
	y = f(b)
	if(y>-d and y<d):
		return b
	if(x<0 and y>0):
		return _eq_half(a,b,f,d)
	if(x>0 and y<0):
		return _eq_half(b,a,f,d)
	return None

# f(a)<0,f(b)>0
def _eq_half(a,b,f,d):
	while True:
		x = 0.5*(a+b)
		y = f(x)
		if(y < 0):
			if(y > -d):
				return x
			a = x
		else:
			if(y < d):
				return x
			b = x
