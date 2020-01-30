class bin_tree( object ):
	def __init__(self):
		self.d = None
		self.l = None
		self.r = None
	def add(self, n, nbit, v):
		p = 1 << (nbit - 1)
		t = self
		while(p != 0):
			c = n & p
			p >>= 1
			if c == 0:
				if t.l == None:
					t.l = bin_tree()
				t = t.l
			else:
				if t.r == None:
					t.r = bin_tree()
				t = t.r
		t.d = v
	def find(self, br):
		t = self
		while(True):
			b = br.read1()
			if b == None:
				break
			if b == 0:
				t = t.l
			else:
				t = t.r
			if t == None:
				break
			if t.d != None:
				return t.d
		return None
