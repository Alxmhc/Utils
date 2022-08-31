class b_int
{
	typedef uint32_t num;
	typedef uint64_t num2;
	
	num n2[2];
	void mul_p(num a, num b, num c = 0)
	{
		num2 t = a;
		t *= b;
		t += c;
		bconv<8, endianness::LITTLE_ENDIAN>::unpack(t, n2);
	}

	std::vector<num> n;

	void del0()
	{
		const auto sz = n.size();
		size_t i = sz;
		while(i != 1 && n[i-1] == 0)
		{
			i--;
		}
		if(i != sz)
		{
			n.resize(i);
		}
	}

	signed char compare(const b_int &a) const
	{
		auto sz = n.size();
		if(a.n.size() != sz)
			return sz > a.n.size() ? 1 : -1;
		while(sz--)
		{
			if(n[sz] != a.n[sz])
				return n[sz] > a.n[sz] ? 1 : -1;
		}
		return 0;
	}

	const b_int& operator-=(const b_int &c)
	{
		if(this == &c
		|| compare(c) == 0)
		{
			*this = 0;
			return *this;
		}
		bool d = false;
		size_t i = 0;
		for(; i < c.n.size(); i++)
		{
			const bool isB = (n[i] >= c.n[i]);
			n[i] -= c.n[i];
			if(d)
			{
				if(isB && n[i] != 0)
				{
					d = false;
				}
				n[i]--;
			}
			else if( !isB )
			{
				d = true;
			}
		}
		while(d)
		{
			if(n[i] != 0)
			{
				d = false;
			}
			n[i]--;
			i++;
		}
		del0();
		return *this;
	}
public:
	b_int(num c = 0) : n(1, c) {}

	const b_int& operator=(num a)
	{
		n = std::vector<num>(1, a);
		return *this;
	}

	void fromB(const uint8_t *v, size_t k)
	{
		n = std::vector<num>((k+3)>>2);
		size_t i = 0;
		while(k > 3)
		{
			k -= 4;
			n[i++] = bconv<4, endianness::BIG_ENDIAN>::pack(v + k);
		}
		if(k != 0)
		{
			uint_fast8_t t[4] = {};
			std::copy_n(v, k, t + 4 - k);
			n[i] = bconv<4, endianness::BIG_ENDIAN>::pack(t);
		}
	}

	std::vector<uint8_t> toB() const
	{
		const size_t k = n.size();
		std::vector<uint8_t> res(k * 4);
		for(size_t i = 0; i < k; i++)
		{
			bconv<4, endianness::BIG_ENDIAN>::unpack(n[i], res.data() + 4*(k-i-1));
		}
		size_t c = 0;
		for(; c < res.size() - 1; c++)
		{
			if(res[c] != 0)
				break;
		}
		if(c != 0)
		{
			res.erase(res.begin(), res.begin() + c);
		}
		return res;
	}

	bool operator==(num a) const
	{
		return (n.size() == 1) && (n[0] == a);
	}
	bool operator!=(num a) const
	{
		return (n.size() != 1) || (n[0] != a);
	}

	const b_int& operator+=(const b_int &c)
	{
		if(this == &c)
			return *this <<= 1;

		auto csz = c.n.size();
		if(n.size() < csz)
		{
			n.resize(csz);
		}

		std::size_t i = 0;
		bool d = false;
		for(; i < csz; i++)
		{
			if(d)
			{
				n[i]++;
				d = (n[i] == 0);
			}
			n[i] += c.n[i];
			d |= (n[i] < c.n[i]);
		}

		if(d)
		{
			const auto sz = n.size();
			for(; i < sz; ++i)
			{
				n[i]++;
				if(n[i] != 0)
					break;
			}
			if(i == sz)
			{
				n.push_back(1);
			}
		}

		return *this;
	}

	const b_int& operator*=(num a)
	{
		if(a == 1 || *this == 0)
			return *this;
		if(a == 0)
		{
			*this = 0;
			return *this;
		}

		std::size_t i = 0;
		while(n[i] == 0)
		{
			i++;
		}

		num d = 0;
		for(const auto sz = n.size(); i < sz; i++)
		{
			mul_p(n[i], a, d);
			n[i] = n2[0];
			d = n2[1];
		}
		if(d)
		{
			n.push_back(d);
		}
		return *this;
	}

	b_int operator*(num a) const
	{
		b_int t(*this);
		return t *= a;
	}

	const b_int& operator*=(const b_int &c)
	{
		b_int res;
		const auto csz = c.n.size();
		res.n.resize(n.size() + csz - 1);

		for(size_t i = 0;;)
		{
			res += *this * c.n[i++];
			if(i == csz)
				break;
			n.insert(n.begin(), 0);
		}

		*this = res;
		return *this;
	}

	b_int operator*(b_int a) const
	{
		b_int t(*this);
		return t *= a;
	}

	const b_int& operator>>=(size_t c)
	{
		auto k = c >> 5;
		if(k != 0)
		{
			n.erase(n.begin(), n.begin() + k);
			c &= 0x1f;
		}
		for(size_t i = 0;;)
		{
			n[i++] >>= c;
			if(i == n.size())
				break;
			n[i-1] |= n[i] << (32 - c);
		}
		del0();
		return *this;
	}

	const b_int& operator<<=(size_t c)
	{
		auto k = c >> 5;
		if(k != 0)
		{
			std::vector<num> tmp(k);
			n.insert(n.begin(), tmp.cbegin(), tmp.cend());
			c &= 0x1f;
		}
		num d = 0;
		for(size_t i = 0; i < n.size(); i++)
		{
			const auto t = n[i] >> (32 - c);
			n[i] = (n[i]<<c) | d;
			d = t;
		}
		if(d != 0)
		{
			n.push_back(d);
		}
		return *this;
	}

	const b_int& operator%=(const b_int &c)
	{
		if( compare(c) < 0 )
			return *this;

		b_int tmp(c);
		for(size_t i = c.n.size(); i < n.size() - 1; i++)
		{
			tmp.n.insert(tmp.n.begin(), 0);
		}
		while(compare(tmp) > 0)
		{
			tmp <<= 1;
		}

		while( c.compare(tmp) <= 0 )
		{
			if(compare(tmp) >= 0)
			{
				operator-=(tmp);
			}
			tmp >>= 1;
		}

		return *this;
	}

	static b_int pw_m(b_int a, num b, b_int c)
	{
		a %= c;
		b_int r(1);
		while(b)
		{
			if(b & 1)
			{
				r *= a;
				r %= c;
			}
			a *= a;
			a %= c;
			if(a == 1)
				break;
			b >>= 1;
		}
		return r;
	}
};
