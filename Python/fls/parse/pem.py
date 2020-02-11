import base64

#text mode
def read(fl):
	str = ""
	for s in fl:
		if s.startswith("-----"):
			if s.startswith("-----END "):
				s = s[9:]
				s = s[:s.index('-')]
				yield [s, base64.b64decode(str)]
				str = ""
			continue
		str += s.rstrip()
