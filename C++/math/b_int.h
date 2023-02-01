template<typename T>
T GCD(T a, T b)
{
	if(b > a)
	{
		b %= a;
	}
	while (b != 0)
	{
		a %= b;
		if(a == 0)
			return b;
		b %= a;
	}
	return a;
}

class b_int
{
	typedef uint32_t num;
	typedef uint64_t num2;
	static const uint_fast8_t BSZ = 32;

	std::vector<num> n;

	signed char compare(const b_int &a) const
	{
		if(this == &a)
			return 0;
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
		if(this == &c)
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
		const auto sz = n.size();
		i = sz;
		while(i != 1 && n[i-1] == 0)
		{
			i--;
		}
		if(i != sz)
		{
			n.resize(i);
		}
		return *this;
	}
public:
	explicit b_int(num c = 0) : n(1, c) {}

	const b_int& operator=(num a)
	{
		n = std::vector<num>(1, a);
		return *this;
	}

	void fromB(const uint8_t* v, size_t k)
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
		return !operator==(a);
	}
	bool operator>(const b_int &c)
	{
		return compare(c) > 0;
	}
	bool operator<(const b_int &c)
	{
		return compare(c) < 0;
	}
	bool operator==(const b_int &c)
	{
		return compare(c) == 0;
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

		size_t i = 0;
		bool d = false;
		for(; i < csz; i++)
		{
			if(d)
			{
				n[i]++;
				d = (n[i] == 0);
			}
			n[i] += c.n[i];
			d = d || (n[i] < c.n[i]);
		}

		if(d)
		{
			for(; i < n.size(); ++i)
			{
				n[i]++;
				if(n[i] != 0)
					return *this;
			}
			n.push_back(1);
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
			num2 t = n[i];
			t *= a;
			t += d;
			num n2[2];
			bconv<8, endianness::LITTLE_ENDIAN>::unpack(t, n2);
			n[i] = n2[0];
			d = n2[1];
		}
		if(d)
		{
			n.push_back(d);
		}
		return *this;
	}

	b_int operator*(num k) const
	{
		b_int r(*this);
		return r *= k;
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
		const auto k = c / BSZ;
		if(k != 0)
		{
			if(k >= n.size())
			{
				*this = 0;
				return *this;
			}
			n.erase(n.begin(), n.begin() + k);
			c %= BSZ;
		}
		if(c == 0)
			return *this;

		const auto sz = n.size();
		for(size_t i = 0;;)
		{
			n[i] >>= c;
			i++;
			if(i == sz)
				break;
			n[i-1] |= n[i] << (BSZ - c);
		}
		if(sz > 1 && n[sz-1] == 0)
		{
			n.resize(sz-1);
		}
		return *this;
	}

	const b_int& operator<<=(size_t c)
	{
		const auto k = c / BSZ;
		if(k != 0)
		{
			std::vector<num> tmp(k);
			n.insert(n.begin(), tmp.cbegin(), tmp.cend());
			c %= BSZ;
		}
		if(c == 0)
			return *this;

		num d = 0;
		for(size_t i = 0; i < n.size(); i++)
		{
			const auto t = n[i] >> (BSZ - c);
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
		auto k = compare(c);
		if( k <= 0 )
		{
			if(k == 0)
			{
				*this = 0;
			}
			return *this;
		}

		b_int tmp(c);
		{
			const auto d = n.size() - c.n.size();
			if(d != 0)
			{
				std::vector<num> v(d);
				tmp.n.insert(tmp.n.begin(), v.cbegin(), v.cend());
			}
		}
		{
			int d = 0;
			const auto nb = n.back();
			num2 cb = tmp.n.back();
			while(cb < nb)
			{
				cb <<= 1;
				d++;
			}
			while(cb > nb)
			{
				cb >>= 1;
				d--;
			}
			if(d < 0)
			{
				tmp >>= (-d);
			}
			else if(d > 0)
			{
				tmp <<= d;
			}
		}

		for(;;)
		{
			k = compare(tmp);
			if(k >= 0)
			{
				if(k == 0)
				{
					*this = 0;
					break;
				}
				operator-=(tmp);
			}
			if(tmp.compare(c) == 0)
				break;
			tmp >>= 1;
		}
		return *this;
	}
};
