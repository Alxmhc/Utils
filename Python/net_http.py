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
		res = ""
		for n, v in self.h.items():
			res += n + ': ' + v + '\r\n'
		return res;

class http_hdr:
	def __init__(self):
		self.hdr = net_header()
	def set_req(self, m, u):
		self.is_out = True
		self.method = m.upper()
		self.URL = u
	def set_resp(self, c, ct):
		self.is_out = False
		self.code = int(c)
		self.code_text = ct
	def add_fld(self, name, val):
		self.hdr.add_fld(name, val)
	def from_text(self, t):
		p = t.find('\r\n')
		self.hdr.from_text(t[p+2:])
		t = t[:p].split(' ', 2)
		if t[0].startswith('HTTP/'):
			self.set_resp(t[1], t[2])
		else:
			self.set_req(t[0], t[1])
	def to_text(self):
		res = ""
		if self.is_out:
			res = self.method + ' ' + self.URL + ' HTTP/1.1\r\n'
		else:
			res = 'HTTP/1.1 ' + str(self.code) + ' ' + self.code_text + '\r\n'
		return res + self.hdr.to_text() + '\r\n'
