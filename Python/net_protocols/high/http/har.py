from . import http
import base64

def from_(obj):
	ver = obj['httpVersion'].upper()
	hdr = {}
	for h in obj['headers']:
		k = h['name']
		if k[0] == ':':
			continue
		http._hdr_add_(hdr, k.lower(), h['value'])
	return ver, hdr
def from_req(obj):
	obj = obj['request']
	ver, hdr = from_(obj)
	inf = {}
	inf['type'] = 0
	inf['method'] = obj['method'].upper()
	url = obj['url']
	url = url[url.find("//")+2:]
	p = url.find("/")
	if not 'host' in hdr:
		hdr['host'] = url[:p]
	inf['url'] = url[p:]
	body = b''
	if 'postData' in obj:
		body = bytes(obj['postData']['text'],'utf8')
	sz = len(body)
	if sz != 0 or inf['method'] == 'POST':
		hdr['content-length'] = str(sz)
	return http.http(inf, ver, hdr, body)
def from_resp(obj):
	obj = obj['response']
	code = obj['status']
	if code == 0:
		return None
	ver, hdr = from_(obj)
	inf = {}
	inf['type'] = 1
	inf['code'] = code
	inf['code_text'] = obj['statusText']
	body = b''
	if 'content' in obj:
		o = obj['content']
		if 'text' in o:
			text = o['text']
			if 'encoding' in o and o['encoding'] == 'base64':
				body = base64.b64decode(text)
			else:
				body = bytes(text,'utf8')
	if 'transfer-encoding' in hdr:
		del hdr['transfer-encoding']
	if 'content-encoding' in hdr:
		del hdr['content-encoding']
	sz = len(body)
	if sz != 0:
		hdr['content-length'] = str(sz)
	return http.http(inf, ver, hdr, body)
