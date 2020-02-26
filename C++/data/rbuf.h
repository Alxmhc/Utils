template<std::size_t SZ>
class rbuf
{
	uint8_t d[SZ];
	std::size_t offset;
public:
	static const std::size_t sz = SZ;
	rbuf() : offset(0) {}
	void clear()
	{
		offset = 0;
		nul();
	}

	template<class C>
	std::size_t process(byteReader &br, C &cl)
	{
		std::size_t r = 0;
		if(offset != 0)
		{
			r = sz - offset;
			const auto rsz = br.read(d + offset, r);
			if(rsz < r)
			{
				offset += rsz;
				return rsz;
			}
			cl.process_block(d);
		}
		for(;;)
		{
			const auto rsz = br.read(d, sz);
			r += rsz;
			if(rsz < sz)
			{
				offset = rsz;
				return r;
			}
			cl.process_block(d);
		}
	}

	template<class C>
	void push(uint8_t c, C &cl)
	{
		d[offset] = c;
		offset++;
		if(offset == sz)
		{
			cl.process_block(d);
			offset = 0;
		}
	}
	const uint8_t* data() const
	{
		return d;
	}
	std::size_t size() const
	{
		return offset;
	}
	std::size_t sz_e() const
	{
		return sz - offset;
	}
	void nul()
	{
		std::fill(d + offset, d + sz, 0);
	}
};
