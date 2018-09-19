class b_int
{
	typedef uint32_t num;
	static const unsigned short int SZ = sizeof(num)<<3;

	class int2
	{
		typedef uint64_t num2;
		int2(num2 c)
		{
			l = c & static_cast<num>(-1);
			h = c >> SZ;
		}

		num2 getc() const
		{
			num2 r = h;
			return (r << SZ) + l;
		}
	public:
		num l, h;
		int2(num a, num b){l=a; h=b;}
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
	b_int(num c = 0){n.push_back(c);}
	b_int(const std::string &str){n = hex::from_str<num>(str);}

	b_int operator=(num c)
	{
		n = std::vector<num>(1, c);
		return *this;
	}

	bool operator==(num c) const
	{
		return (n.size() == 1) && (n[0] == c);
	}
	bool operator==(const b_int &c) const
	{
		return n == c.n;
	}

	bool operator>(const b_int &c) const
	{
		auto sz = c.n.size();
		if(n.size() != sz)
			return n.size() > sz;
		while(sz--)
		{
			if(n[sz] != c.n[sz])
				return n[sz] > c.n[sz];
		}
		return false;
	}

	bool operator<(const b_int &c) const
	{
		auto sz = c.n.size();
		if(n.size() != sz)
			return n.size() < sz;
		while(sz--)
		{
			if(n[sz] != c.n[sz])
				return n[sz] < c.n[sz];
		}
		return false;
	}

	b_int operator*=(num a)
	{
		if(*this == 0 || a == 1)
			return *this;
		if(a == 0)
		{
			*this = 0;
			return *this;
		}
		std::size_t i = 0;
		while(n[i] == 0){i++;}
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

	b_int operator>>=(num d)
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

	b_int operator<<=(num d)
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