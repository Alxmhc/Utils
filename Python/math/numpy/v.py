def cntr(m):
	r  = m.mean(0)
	m -= r
	return r

def appr_OLS(x, y, k):
	#left part
	m = numpy.empty([k+1,k+1])
	r = numpy.empty(2*k+1)
	s = numpy.copy(x)
	r[0] = float(len(x))
	r[1] = s.sum()
	for i in range(2,2*k+1):
		s *= x
		r[i] = s.sum()
	for i in range(k+1):
		m[i] = r[i:i+k+1]
	#right part
	r = numpy.empty(k+1)
	s = numpy.copy(y)
	r[0] = s.sum()
	for i in range(1,k+1):
		s *= x
		r[i] = s.sum()
	rz = numpy.linalg.solve(m,r)
	return numpy.polynomial.polynomial.Polynomial(rz)
