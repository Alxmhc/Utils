import math

def is_sqr(c):
	if c < 4:
		return c == 0 or c == 1
	while c & 3 == 0:
		c >>= 2
	if c & 7 != 1:
		return False
	t = math.isqrt(c)
	return c == t * t

#min divisor (c > 1)
def minDiv(c):
	if c&1 == 0:
		if c == 2:
			return 1
		return 2
	q = math.isqrt(c) + 1
	o = 3
	while(o < q):
		if(c%o == 0):
			return o
		o += 2
	return 1
