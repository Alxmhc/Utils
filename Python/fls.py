import os

def proc_dir(pth, fncF, fncD = None):
	for r, dirs, fls in os.walk(pth):
		r = r.replace('\\','/') + '/'
		if fncD != None:
			for d in dirs:
				fncD(r + d + '/')
		if fncF != None:
			for fl in fls:
				fncF(r + fl)
