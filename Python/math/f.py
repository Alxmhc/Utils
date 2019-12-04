def gcd(a,b):
	while(b):
		a, b = b, a%b
	return a

#r*r <= c
def sqrt_int(c):
	if c < 2:
		if c < 0:
			return None
		return c
	r = 2 << (c.bit_length() >> 1)
	t = (r + c // r) >> 1
	while t < r:
		r, t = t, (t + c // t) >> 1
	return r

def is_sqr(c):
	if c < 2:
		return c >= 0
	while c & 3 == 0:
		c >>= 2
	if c & 7 != 1:
		return False
	c >>= 2
	t = sqrt_int(c)
	return c == t*(t + 1)
