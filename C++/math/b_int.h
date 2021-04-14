class b_int
{
	typedef uint32_t num;
	static const unsigned short int SZ = sizeof(num)<<3;

	class int2
	{
		typedef uint64_t num2;
		int2(num2 c)
		{
			l = static_cast<num>(c);
			h = c >> SZ;
		}

		num2 getc() const
		{
			num2 r = h;
			return (r << SZ) + l;
		}
	public:
		num l, h;
		int2(num a, num b) : l(a), h(b) {}
		//a*b + c
		static int2 mul_p(num a, num b, num c = 0)
		{
			num2 t = a;
			t *= b;
			t += c;
			return int2(t);
		}

		//(a^b)%c
		static num pw_m(num a, num b, num c)
		{
			num2 t = a, r = 1;
			while(b)
			{
				if(b & 1)
				{
					r *= t;
					r %= c;
				}
				t *= t;
				t %= c;
				if(t == 1)
					break;
				b >>= 1;
			}
			return static_cast<num>(r);
		}

		num operator%(num a) const
		{
			return getc() % a;
		}
	};

	std::vector<num> n;
public:
	b_int(num c = 0) : n(1, c) {}
	b_int(const std::string &str)
	{
		n = hex::from_str<num>(str);
	}

	const b_int& operator=(num a)
	{
		n = std::vector<num>(1, a);
		return *this;
	}

	bool operator>(num a) const
	{
		return (n.size() > 1) || (n[0] > a);
	}
	bool operator<(num a) const
	{
		return (n.size() == 1) && (n[0] < a);
	}
	bool operator==(num a) const
	{
		return (n.size() == 1) && (n[0] == a);
	}

	const b_int& operator+=(num a)
	{
		n[0] += a;
		if(n[0] < a)
		{
			size_t i = 1;
			auto sz = n.size();
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
		if(*this == 0 || a == 1)
			return *this;
		if(a == 0)
		{
			*this = 0;
			return *this;
		}
		size_t i = 0;
		while(n[i] == 0)
		{
			i++;
		}
		num d = 0;
		for(auto sz = n.size(); i < sz; i++)
		{
			auto t = int2::mul_p(n[i], a, d);
			n[i] = t.l;
			d = t.h;
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


	friend signed char compare(const b_int &a, const b_int &b);
	bool operator>(const b_int &c) const
	{
		return compare(*this, c) > 0;
	}
	bool operator<(const b_int &c) const
	{
		return compare(*this, c) < 0;
	}
	bool operator==(const b_int &c) const
	{
		return n == c.n;
	}

	const b_int& operator+=(const b_int &c)
	{
		auto csz = c.n.size();
		if(csz == 1)
		{
			operator+=(c.n[0]);
			return *this;
		}
		auto sz = n.size();
		if(sz == 1)
		{
			auto tmp = n[0];
			n = c.n;
			operator+=(tmp);
			return *this;
		}
		if(sz < csz)
		{
			n.resize(csz);
			sz = csz;
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
			d |= (n[i] < c.n[i]);
		}
		if(d)
		{
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

	const b_int& operator*=(const b_int &c)
	{
		auto csz = c.n.size();
		if(csz == 1)
		{
			operator*=(c.n[0]);
			return *this;
		}
		auto sz = n.size();
		if(sz == 1)
		{
			auto tmp = n[0];
			n = c.n;
			operator*=(tmp);
			return *this;
		}
		auto tmp = *this;
		n.reserve(sz + csz);
		operator*=(c.n[0]);
		for(size_t i = 1; i < csz; ++i)
		{
			tmp.n.insert(tmp.n.begin(), 0);
			operator+=(tmp * c.n[i]);
		}
		return *this;
	}

	b_int operator*(const b_int &c) const
	{
		b_int t(*this);
		return t *= c;
	}


	num operator%(num a) const
	{
		auto i = n.size();
		int2 r(0, 0);
		while(i--)
		{
			r.l = n[i];
			r.h = r % a;
		}
		return r.h;
	}

	//(n^p)%a
	num pw_m(num p, num a) const
	{
		return int2::pw_m(operator%(a), p, a);
	}

	const b_int& operator>>=(num d)
	{
		if(*this == 0)
			return *this;
		if(d >= SZ)
		{
			auto p = d/SZ;
			if(p >= n.size())
			{
				*this = 0;
				return *this;
			}
			n.erase(n.begin(), n.begin() + p);
			d -= p*SZ;
		}
		if(d == 0)
			return *this;

		auto sz = n.size() - 1;
		for(num i = 0; i < sz; i++)
		{
			n[i] = (n[i]>>d) | (n[i+1]<<(SZ-d));
		}
		n[sz] >>= d;
		if(sz != 0 && n[sz] == 0)
		{
			n.erase(n.end()-1, n.end());
		}
		return *this;
	}

	const b_int& operator<<=(num d)
	{
		if(*this == 0)
			return *this;
		num p = 0;
		if(d >= SZ)
		{
			p = d/SZ;
			const std::vector<num> v(p, 0);
			n.insert(n.begin(), v.cbegin(), v.cend());
			d -= p*SZ;
		}
		if(d == 0)
			return *this;
		while(n[p] == 0){p++;}
		auto sz = n.size() - 1;
		{
			num o = n[sz]>>(SZ-d);
			if(o != 0)
			{
				n.push_back(o);
			}
		}
		for(; sz>p; sz--)
		{
			n[sz] = (n[sz]<<d) | (n[sz-1]>>(SZ-d));
		}
		n[p] <<= d;
		return *this;
	}
};

signed char compare(const b_int &a, const b_int &b)
{
	auto sz = b.n.size();
	if(a.n.size() != sz)
		return a.n.size()>sz ? 1 : -1;
	while(sz--)
	{
		if(a.n[sz] != b.n[sz])
			return a.n[sz]>b.n[sz] ? 1 : -1;
	}
	return 0;
}
