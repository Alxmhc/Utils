import base64

#text mode
def read(fl):
	str = ""
	k = ""
	for s in f:
		if s.startswith("-----BEGIN "):
			k = s[11:]
			k = k[:k.index('-')]
			continue
		if not k:
			continue
		if s.startswith("-----END "):
			yield [k, base64.b64decode(str)]
			str = ""
			k = ""
			continue
		str += s.rstrip()
