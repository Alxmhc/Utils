#ifndef H_B_INT
#define H_B_INT

#include <vector>
#include <algorithm>

#include "base/math_.h"
#include "../data/pack.h"

class b_uint
{
	typedef uint32_t num;
	typedef uint64_t num2;
	static const uint_fast8_t BSZ = 32;

	std::vector<num> n;

	void fix()
	{
		if(n.back() != 0
		|| n.size() == 1)
			return;
		auto sz = n.size() - 2;
		while(sz != 0 && n[sz] == 0)
		{
			sz--;
		}
		n.resize(sz + 1);
	}

	static signed char compare(const b_uint &a, const b_uint &b)
	{
		auto sz = a.n.size();
		{
			const auto csz = b.n.size();
			if(sz != csz)
				return sz > csz ? 1 : -1;
		}
		while(sz--)
		{
			if(a.n[sz] != b.n[sz])
				return a.n[sz] > b.n[sz] ? 1 : -1;
		}
		return 0;
	}

	void add(num c, std::size_t p)
	{
		n[p] += c;
		if(n[p] < c)
		{
			const auto sz = n.size();
			for(std::size_t i = p + 1; ;i++)
			{
				if(i == sz)
				{
					n.push_back(1);
					break;
				}
				n[i]++;
				if(n[i] != 0)
					break;
			}
		}
	}
public:
	explicit b_uint(num c = 0) : n(1, c) {}

	const b_uint& operator=(num a)
	{
		n.resize(1);
		n[0] = a;
		return *this;
	}

	void fromB(const uint8_t* v, std::size_t k)
	{
		while(k != 0 && *v == 0)
		{
			v++;
			k--;
		}
		if(k == 0)
		{
			*this = 0;
			return;
		}

		n.resize((k+3)>>2);
		std::size_t i = 0;
		while(k > 3)
		{
			k -= 4;
			n[i] = bconv<1, 4, endianness::BIG_ENDIAN>::pack(v + k);
			i++;
		}
		if(k == 0)
			return;

		n[i] = 0;
		while(k--)
		{
			n[i] = (n[i] << 8) | *v;
			v++;
		}
	}

	std::vector<uint8_t> toB() const
	{
		const std::size_t k = n.size();
		std::vector<uint8_t> res(k * 4);
		for(std::size_t i = 0; i < k; i++)
		{
			bconv<1, 4, endianness::BIG_ENDIAN>::unpack(n[i], res.data() + 4*(k-i-1));
		}
		std::size_t c = 0;
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

	bool operator==(const b_uint &c) const
	{
		return compare(*this, c) == 0;
	}
	bool operator!=(const b_uint &c) const
	{
		return !operator==(c);
	}
	bool operator>(const b_uint &c) const
	{
		return compare(*this, c) > 0;
	}
	bool operator<(const b_uint &c) const
	{
		return compare(*this, c) < 0;
	}

	uint_fast8_t getBit(std::size_t p) const
	{
		const auto k = p/BSZ;
		if(k < n.size())
			return ::getBit(n[k], p%BSZ);
		return 0;
	}
	void setBit(std::size_t p, bool b)
	{
		const auto k = p/BSZ;
		if(k < n.size())
		{
			::setBit(n[k], p%BSZ, b);
			if(!b)
			{
				fix();
			}
		}
		else if(b)
		{
			n.resize(k + 1);
			::setBit(n[k], p%BSZ, true);
		}
	}

	// % (1<<c)
	b_uint get_bits(std::size_t c) const
	{
		if(c == 0)
			return b_uint();
		std::size_t sz = (c-1)/BSZ + 1;
		if(sz > n.size())
			return *this;

		b_uint res;
		res.n.assign(n.cbegin(), n.cbegin() + sz);
		const uint_fast8_t d = c % BSZ;
		if(d != 0)
		{
			res.n.back() &= (1<<d) - 1;
		}

		res.fix();
		return res;
	}

	std::size_t log2i() const
	{
		std::size_t res = (n.size() - 1) * BSZ;
		return res + ::log2i(n.back());
	}

	num operator&(num c) const
	{
		return n[0] & c;
	}

	num operator%(num c) const
	{
		if((c & (c - 1)) == 0)
			return n[0] & (c - 1);
		auto i = n.size();
		num2 r = 0;
		while(i--)
		{
			r = ((r << BSZ) | n[i]) % c;
		}
		return static_cast<num>(r);
	}

	const b_uint& operator>>=(std::size_t c)
	{
		if(*this == 0)
			return *this;
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
		}
		if(c == 0)
			return *this;

		const auto sz = n.size() - 1;
		for(std::size_t i = 0; i != sz; i++)
		{
			n[i] >>= c;
			n[i] |= n[i+1] << (BSZ - c);
		}
		n[sz] >>= c;

		if(sz != 0 && n[sz] == 0)
		{
			n.resize(sz);
		}
		return *this;
	}

	const b_uint& operator<<=(std::size_t c)
	{
		if(*this == 0)
			return *this;
		std::size_t i = 0;
		{
			const auto k = c / BSZ;
			if(k != 0)
			{
				i += k;
				std::vector<num> tmp(k);
				n.insert(n.begin(), tmp.cbegin(), tmp.cend());
				c %= BSZ;
			}
		}
		if(c == 0)
			return *this;

		num d = 0;
		for(; i < n.size(); i++)
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

	const b_uint& operator+=(num c)
	{
		add(c, 0);
		return *this;
	}

	//*this >= c
	const b_uint& operator-=(num c)
	{
		bool d = n[0] < c;
		n[0] -= c;
		for(std::size_t i = 1; d; i++)
		{
			d = n[i] == 0;
			n[i]--;
		}
		if(n.size() != 1 && n.back() == 0)
		{
			n.pop_back();
		}
		return *this;
	}

	const b_uint& operator*=(num c)
	{
		if(c == 1)
			return *this;
		if(c == 0)
		{
			*this = 0;
			return *this;
		}

		num d = 0;
		for(std::size_t i = 0; i < n.size(); i++)
		{
			num2 t = n[i];
			t = t*c + d;
			n[i] = static_cast<num>(t);
			d = t >> BSZ;
		}
		if(d)
		{
			n.push_back(d);
		}
		return *this;
	}

	const b_uint& operator+=(const b_uint &c)
	{
		const auto csz = c.n.size();
		if(n.size() < csz)
		{
			n.resize(csz);
		}

		std::size_t i = 0;
		num2 d = 0;
		for(; i < csz; i++)
		{
			d += n[i];
			d += c.n[i];
			n[i] = static_cast<num>(d);
			d >>= BSZ;
		}
		while(d != 0)
		{
			if(i == n.size())
			{
				n.push_back(static_cast<num>(d));
				break;
			}
			d += n[i];
			n[i] = static_cast<num>(d);
			d >>= BSZ;
			i++;
		}
		return *this;
	}

	b_uint operator*(const b_uint &c) const
	{
		b_uint res;
		res.n.resize(n.size() + c.n.size());
		for(std::size_t i = 0; i < c.n.size(); i++)
		{
			for(std::size_t j = 0; j < n.size(); j++)
			{
				num2 t = c.n[i];
				t *= n[j];
				res.add(static_cast<num>(t), i + j);
				res.add(t >> BSZ, i + j + 1);
			}
		}
		res.fix();
		return res;
	}

	//*this >= c
	const b_uint& operator-=(const b_uint &c)
	{
		std::size_t i = 0;

		bool d = false;
		for(; i < c.n.size(); i++)
		{
			if(d)
			{
				d = (n[i] <= c.n[i]);
				n[i]--;
			}
			else
			{
				d = (n[i] < c.n[i]);
			}
			n[i] -= c.n[i];
		}
		while(d)
		{
			d = (n[i] == 0);
			n[i]--;
			i++;
		}

		fix();
		return *this;
	}

	const b_uint& ModDiv(b_uint c, b_uint* r)
	{
		if(r != nullptr)
		{
			*r = 0;
		}
		if(*this < c)
			return *this;
		if(c.n.size() == 1)
		{
			if(r == nullptr)
			{
				*this = *this % c.n[0];
				return *this;
			}
			else if(c.n[0] == 1)
			{
				std::swap(*this, *r);
				return *this;
			}
		}

		std::size_t d = log2i() - c.log2i() + 1;
		c <<= d;
		while(d--)
		{
			c >>= 1;
			if(*this < c)
				continue;
			if(r != nullptr)
			{
				r->setBit(d, true);
			}
			*this -= c;
		}
		return *this;
	}

	const b_uint& operator%=(const b_uint &c)
	{
		return ModDiv(c, nullptr);
	}
};

class b_sint
{
	b_uint u;
	bool p; //>=0
public:
	b_sint() : p(true) {}

	b_sint(const b_uint &c, bool pos) : u(c)
	{
		p = pos ? true : c == 0;
	}

	const b_uint& abs() const
	{
		return u;
	}

	b_uint mod(const b_uint &m) const
	{
		b_uint res(u);
		res %= m;
		if(!p && res != 0)
		{
			res = m - res;
		}
		return res;
	}

	const b_sint& operator+=(const b_uint &c)
	{
		if(p)
		{
			u += c;
		}
		else if(u > c)
		{
			u -= c;
		}
		else
		{
			u = c - u;
			p = true;
		}
		return *this;
	}

	const b_sint& operator-=(const b_uint &c)
	{
		if(!p)
		{
			u += c;
		}
		else if(u < c)
		{
			u = c - u;
			p = false;
		}
		else
		{
			u -= c;
		}
		return *this;
	}

	const b_sint& operator+=(const b_sint &c)
	{
		return c.p ? *this += c.u : *this -= c.u;
	}

	const b_sint& operator-=(const b_sint &c)
	{
		return c.p ? *this -= c.u : *this += c.u;
	}

	b_sint operator*(const b_sint &c) const
	{
		return b_sint(u * c.u, p == c.p);
	}
};

#endif
