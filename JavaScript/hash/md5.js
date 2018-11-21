class MD5
{
	constructor(){
		this.b = new rbuf(64);
		this.st = new Uint32Array(4);
		this.Clear();
	}
	Clear()
	{
		this.b.clear()
		this.st.set([0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476])
		this.size = 0
	}
	process_block(buf)
	{
		const x = conv.pack_le(buf)
		this.Transform(x)
	}
	Transform(x)
	{
		let a=this.st[0], b=this.st[1], c=this.st[2], d=this.st[3]

		a = MD5.FF(a, b, c, d, x[ 0],  7, 0xd76aa478)
		d = MD5.FF(d, a, b, c, x[ 1], 12, 0xe8c7b756)
		c = MD5.FF(c, d, a, b, x[ 2], 17, 0x242070db)
		b = MD5.FF(b, c, d, a, x[ 3], 22, 0xc1bdceee)
		a = MD5.FF(a, b, c, d, x[ 4],  7, 0xf57c0faf)
		d = MD5.FF(d, a, b, c, x[ 5], 12, 0x4787c62a)
		c = MD5.FF(c, d, a, b, x[ 6], 17, 0xa8304613)
		b = MD5.FF(b, c, d, a, x[ 7], 22, 0xfd469501)
		a = MD5.FF(a, b, c, d, x[ 8],  7, 0x698098d8)
		d = MD5.FF(d, a, b, c, x[ 9], 12, 0x8b44f7af)
		c = MD5.FF(c, d, a, b, x[10], 17, 0xffff5bb1)
		b = MD5.FF(b, c, d, a, x[11], 22, 0x895cd7be)
		a = MD5.FF(a, b, c, d, x[12],  7, 0x6b901122)
		d = MD5.FF(d, a, b, c, x[13], 12, 0xfd987193)
		c = MD5.FF(c, d, a, b, x[14], 17, 0xa679438e)
		b = MD5.FF(b, c, d, a, x[15], 22, 0x49b40821)

		a = MD5.GG(a, b, c, d, x[ 1],  5, 0xf61e2562)
		d = MD5.GG(d, a, b, c, x[ 6],  9, 0xc040b340)
		c = MD5.GG(c, d, a, b, x[11], 14, 0x265e5a51)
		b = MD5.GG(b, c, d, a, x[ 0], 20, 0xe9b6c7aa)
		a = MD5.GG(a, b, c, d, x[ 5],  5, 0xd62f105d)
		d = MD5.GG(d, a, b, c, x[10],  9, 0x02441453)
		c = MD5.GG(c, d, a, b, x[15], 14, 0xd8a1e681)
		b = MD5.GG(b, c, d, a, x[ 4], 20, 0xe7d3fbc8)
		a = MD5.GG(a, b, c, d, x[ 9],  5, 0x21e1cde6)
		d = MD5.GG(d, a, b, c, x[14],  9, 0xc33707d6)
		c = MD5.GG(c, d, a, b, x[ 3], 14, 0xf4d50d87)
		b = MD5.GG(b, c, d, a, x[ 8], 20, 0x455a14ed)
		a = MD5.GG(a, b, c, d, x[13],  5, 0xa9e3e905)
		d = MD5.GG(d, a, b, c, x[ 2],  9, 0xfcefa3f8)
		c = MD5.GG(c, d, a, b, x[ 7], 14, 0x676f02d9)
		b = MD5.GG(b, c, d, a, x[12], 20, 0x8d2a4c8a)

		a = MD5.HH(a, b, c, d, x[ 5],  4, 0xfffa3942)
		d = MD5.HH(d, a, b, c, x[ 8], 11, 0x8771f681)
		c = MD5.HH(c, d, a, b, x[11], 16, 0x6d9d6122)
		b = MD5.HH(b, c, d, a, x[14], 23, 0xfde5380c)
		a = MD5.HH(a, b, c, d, x[ 1],  4, 0xa4beea44)
		d = MD5.HH(d, a, b, c, x[ 4], 11, 0x4bdecfa9)
		c = MD5.HH(c, d, a, b, x[ 7], 16, 0xf6bb4b60)
		b = MD5.HH(b, c, d, a, x[10], 23, 0xbebfbc70)
		a = MD5.HH(a, b, c, d, x[13],  4, 0x289b7ec6)
		d = MD5.HH(d, a, b, c, x[ 0], 11, 0xeaa127fa)
		c = MD5.HH(c, d, a, b, x[ 3], 16, 0xd4ef3085)
		b = MD5.HH(b, c, d, a, x[ 6], 23, 0x04881d05)
		a = MD5.HH(a, b, c, d, x[ 9],  4, 0xd9d4d039)
		d = MD5.HH(d, a, b, c, x[12], 11, 0xe6db99e5)
		c = MD5.HH(c, d, a, b, x[15], 16, 0x1fa27cf8)
		b = MD5.HH(b, c, d, a, x[ 2], 23, 0xc4ac5665)

		a = MD5.II(a, b, c, d, x[ 0],  6, 0xf4292244)
		d = MD5.II(d, a, b, c, x[ 7], 10, 0x432aff97)
		c = MD5.II(c, d, a, b, x[14], 15, 0xab9423a7)
		b = MD5.II(b, c, d, a, x[ 5], 21, 0xfc93a039)
		a = MD5.II(a, b, c, d, x[12],  6, 0x655b59c3)
		d = MD5.II(d, a, b, c, x[ 3], 10, 0x8f0ccc92)
		c = MD5.II(c, d, a, b, x[10], 15, 0xffeff47d)
		b = MD5.II(b, c, d, a, x[ 1], 21, 0x85845dd1)
		a = MD5.II(a, b, c, d, x[ 8],  6, 0x6fa87e4f)
		d = MD5.II(d, a, b, c, x[15], 10, 0xfe2ce6e0)
		c = MD5.II(c, d, a, b, x[ 6], 15, 0xa3014314)
		b = MD5.II(b, c, d, a, x[13], 21, 0x4e0811a1)
		a = MD5.II(a, b, c, d, x[ 4],  6, 0xf7537e82)
		d = MD5.II(d, a, b, c, x[11], 10, 0xbd3af235)
		c = MD5.II(c, d, a, b, x[ 2], 15, 0x2ad7d2bb)
		b = MD5.II(b, c, d, a, x[ 9], 21, 0xeb86d391)

		this.st[0] += a
		this.st[1] += b
		this.st[2] += c
		this.st[3] += d
	}
	Update(v)
	{
		this.size += v.length
		this.b.process(v, this)
	}
	Final()
	{
		this.b.push(0x80)
		this.b.nul()
		let x = conv.pack_le(this.b.buf)
		if(this.b.sz_e() < 8)
		{
			this.Transform(x)
			x.fill(0)
		}
		x[14] = this.size<<3
		x[15] = this.size>>>29
		this.Transform(x)

		return conv.unpack_le(this.st)
	}
}
MD5.FF = function(a,b,c,d,x,s,ac){return rotl(a + ((b&c)|(~b&d)) + x + ac, s) + b}
MD5.GG = function(a,b,c,d,x,s,ac){return rotl(a + ((b&d)|(c&~d)) + x + ac, s) + b}
MD5.HH = function(a,b,c,d,x,s,ac){return rotl(a + (b^c^d) + x + ac, s) + b}
MD5.II = function(a,b,c,d,x,s,ac){return rotl(a + (c^(b|~d)) + x + ac, s) + b}
