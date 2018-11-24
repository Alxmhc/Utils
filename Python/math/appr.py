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
