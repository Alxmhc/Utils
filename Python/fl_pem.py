import base64

def read(pth):
	str = ""
	fl = open(pth, "r")
	for s in fl:
		if s.startswith("-----"):
			if s.startswith("-----END "):
				s = s[9:]
				s = s[:s.index('-')]
				yield [s, base64.b64decode(str)]
				str = ""
		else:
			str += s.rstrip()
	fl.close()
