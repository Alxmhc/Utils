class sph( object ):
	def __init__(self, r):
		self.r = r
	@staticmethod
	def vl_(r):
		return (4.0/3.0)*math.pi*r*r*r
	def vl(self):
		return sph.vl_(self.r)
	#volume cut off by a plane at a distance d from the center
	def vl_p(self, d):
		if(d < self.r):
			return (math.pi/3.0) * ( self.r*self.r*(2*self.r-3*d) + d*d*d )
		return 0.0
