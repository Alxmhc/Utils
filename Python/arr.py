def part_reverse(a, p1, p2):
	for i in range(0, (p2 - p1 + 1)//2):
		a[p1+i], a[p2-i] = a[p2-i], a[p1+i]

def nxt_perm(a):
	if len(a) < 2:
		return False
	i = len(a)-1
	while True:
		if a[i] > a[i-1]:
			break
		i -= 1
		if i == 0:
			return False
	part_reverse(a, i, len(a)-1)
	j = i-1
	while True:
		if a[i] > a[j]:
			break
		i += 1
	a[i], a[j] = a[j], a[i]
	return True
