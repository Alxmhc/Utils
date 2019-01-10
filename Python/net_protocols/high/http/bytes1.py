from . import http

def from_bytes_(frame):
	p = frame.find(b"\r\n\r\n")
	body = frame[p+4:]
	frame = frame[:p].decode('utf8')
	p = frame.find("\r\n")
	hdr = {}
	for h in frame[p+2:].split("\r\n"):
		d = h.find(": ")
		http._hdr_add_(hdr, h[:d].lower(), h[d+2:])
	ln = frame[:p].split(' ', 2)
	ln[0] = ln[0].upper()
	return ln, hdr, body
def from_bytes_req(frame):
	ln, hdr, body = from_bytes_(frame)
	ver = ln[2].upper()
	inf = {}
	inf['type'] = 0
	inf['method'] = ln[0]
	inf['url'] = ln[1]
	return http.http(inf, ver, hdr, body)
def from_bytes_resp(frame):
	ln, hdr, body = from_bytes_(frame)
	ver = ln[0]
	inf = {}
	inf['type'] = 1
	inf['code'] = ln[1]
	inf['code_text'] = ln[2]
	return http.http(inf, ver, hdr, body)

def to_bytes(h):
	if h.type == 0:
		res = h.method + " " + h.url + " " + h.ver + "\r\n"
	else:
		res = h.ver + " " + str(h.code) + " " + h.code_text + "\r\n"
	for k,v in h.hdr.items():
		t = k.title() + ": "
		if not isinstance(v, list):
			res += t + v + "\r\n"
		else:
			for e in v:
				res += t + e + "\r\n"
	res += "\r\n"
	return bytes(res, 'utf8') + h.body
