class net_header:
	def __init__(self):
		self.h = {}
	def add_fld(self, name, val):
		name = name.lower()
		if name in self.h:
			self.h[name] += '; ' + val
		else:
			self.h[name] = val
	def get_fld(self, name):
		name = name.lower()
		if name in self.h:
			return self.h[name]
		return None
	def from_text(self, t):
		self.h = {}
		for h in t.split('\r\n'):
			p = h.find(': ')
			self.add_fld(h[:p], h[p+2:])
	def to_text(self):
		res = ''
		for n, v in self.h.items():
			res += n + ': ' + v + '\r\n'
		return res;

class http_hdr:
	def __init__(self, is_out):
		self.is_out = is_out
		self.s1 = ''
		self.s2 = ''
		self.hdr = net_header()
	def add_fld(self, name, val):
		self.hdr.add_fld(name, val)
	def get_fld(self, name):
		return self.hdr.get_fld(name)

class HTTP1:
	def __init__(self, is_out):
		self.hdr = http_hdr(is_out)
		self.body = b''
	@staticmethod
	def hdr_from_text(t):
		h = http_hdr(not t.startswith('HTTP/'))
		p = t.find('\r\n')
		l = t[:p].split(' ', 2)
		if h.is_out:
			h.s1 = l[0]
			h.s2 = l[1]
		else:
			h.s1 = l[1]
			h.s2 = l[2]
		h.hdr.from_text(t[p+2:])
		return h
	@staticmethod
	def hdr_to_text(h):
		if h.is_out:
			res = h.s1 + ' ' + h.s2 + ' HTTP/1.1\r\n'
		else:
			res = 'HTTP/1.1 ' + h.s1 + ' ' + h.s2 + '\r\n'
		return res + h.hdr.to_text() + '\r\n'
