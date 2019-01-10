def _hdr_add_(hdr, k, v):
	if not k in hdr:
		hdr[k] = v
	elif isinstance(hdr[k], list):
		hdr[k].append(v)
	else:
		hdr[k] = [hdr[k], v]

class http( object ):
	def __init__(self, inf, ver, hdr, body):
		self.ver = ver
		self.hdr = hdr
		self.body = body
		self.type = inf['type']
		if self.type == 0 :
			self.method = inf['method']
			self.url = inf['url']
		else:
			self.code = inf['code']
			self.code_text = inf['code_text']
