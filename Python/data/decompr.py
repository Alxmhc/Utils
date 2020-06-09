def LZ77_repeat(sz, dist, out):
	buf = out[len(out) - dist:]
	if(sz >= dist):
		k = sz // dist
		out += buf * k
		sz -= dist * k
	if(sz > 0):
		out += buf[ : sz]
