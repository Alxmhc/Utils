def f_rb(fl):
	f = open(fl,'rb')
	r = f.read()
	f.close()
	return r

def f_wb(fl,r):
	f = open(fl,'wb')
	f.write(r)
	f.close()
