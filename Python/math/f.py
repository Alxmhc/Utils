def gcd(a,b):
	while(b):a,b = b,a%b
	return a

def fct(n):
	return fctp(2,n)

def fctp(n,m):
	t = range(n, m+1)
	return pr(t)

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
	if(c&1 == 0):
		return 2
	return min_pr_b(c)

# r*r <= a; (r+1)*(r+1) > a
def sqrt_int(a):
	r = 0
	t = 1<<(a.bit_length()>>1)
	while t:
		if (r+t)*(r+t) <= a:
			r += t
		t >>= 1
	return r

#odd > 1
#not optimized
def min_pr_b(c, o = 3):
	if c == o:
		return 1
	q = sqrt_int(c)+1
	while(o < q):
		if(c%o == 0):
			return o
		o += 2
	return 1
