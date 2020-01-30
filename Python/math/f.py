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
