import math

def min_pr(c):
	if c < 4:
		return 1
	if c%2 == 0:
		return 2
	m = math.isqrt(c) + 1
	p = 3
	while p < m:
		if c%p == 0:
			return p
		p += 2
	return 1
