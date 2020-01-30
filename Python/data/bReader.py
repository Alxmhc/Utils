class byteReader( object ):
	def __init__(self, d):
		self._d = d
		self._o = 0
	def get(self):
		if self._o >= len(self._d):
			return None
		r = self._d[self._o]
		self._o += 1
		return r

class bitReader( object ):
	def __init__(self, d):
		self.__br = byteReader(d)
		self._b = None
		self._o = 0
	def getB(self):
		self._b = self.__br.get()

#01234567
class bitReaderR( bitReader ):
	def read1(self):
		if self._o != 0:
			r = (self._b >> (7 - self._o)) & 1
			self._o = (self._o + 1) & 7
			return r
		self.getB()
		if self._b == None:
			return None
		self._o = 1
		return self._b >> 7
	def readBE(self, n):
		r = 0
		for i in range(n):
			r = (r<<1) | self.read1()
		return r
	def readLE(self, n):
		r = 0
		for i in range(n):
			r |= self.read1()<<i
		return r

#76543210
class bitReaderL( bitReader ):
	def read1(self):
		if self._o != 0:
			r = (self._b >> self._o) & 1
			self._o = (self._o + 1) & 7
			return r
		self.getB()
		if self._b == None:
			return None
		self._o = 1
		return self._b & 1
	def readBE(self, n):
		r = 0
		for i in range(n):
			r |= self.read1()<<i
		return r
	def readLE(self, n):
		r = 0
		for i in range(n):
			r = (r<<1) | self.read1()
		return r
