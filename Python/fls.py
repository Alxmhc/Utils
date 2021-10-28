import os

def dir_(dir, depth):
	l = [(dir,depth)]
	while(l):
		dir, depth = l.pop()
		try:
			lst = os.listdir(dir)
		except Exception:
			continue
		for t in lst:
			e = os.path.join(dir, t)
			is_dir = os.path.isdir(e)
			if is_dir and depth != 0:
				l.append((e, depth-1))
			yield e, is_dir

def dir_files(dir, depth = -1):
	return (p for p,f in dir_(dir, depth) if not f)

def dir_folders(dir, depth = -1):
	return (p for p,f in dir_(dir, depth) if f)
