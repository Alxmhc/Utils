def arr_pr(m):
	r = 1
	for c in m:
		r *= c
	return r

def gcd(a,b):
	while(b):a,b = b,a%b
	return a

def fct(n):
	return fctp(2,n)

def fctp(n,m):
	t = range(n, m+1)
	return arr_pr(t)

def cf_bn(n,k):
	if(k<n/2):
		return fctp(n-k+1,n)//fct(k)
	return fctp(k+1,n)//fct(n-k)

#min divisor (1 for prime, 0 for 0,1)
def min_pr(c):
	if(c < 4):
		if(c < 2):
			return 0
		return 1
	if(not(c&1)):
		return 2
	return min_pr_b(c)

#odd > 1
#not optimized
def min_pr_b(c, o = 3):
	if c == o:
		return 1
	q = math.floor(math.sqrt(c))+1
	while(o < q):
		if(c%o == 0):
			return o
		o += 2
	return 1
