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
		if(k != 0)
		{
			uint_fast8_t t[4] = {};
			std::copy_n(v, k, t + 4 - k);
			n[i] = bconv<1, 4, endianness::BIG_ENDIAN>::pack(t);
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

		const auto sz = n.size();
		for(std::size_t i = 0;;)
		{
			n[i] >>= c;
			i++;
			if(i == sz)
				break;
			n[i-1] |= n[i] << (BSZ - c);
		}
		if(n.back() == 0 && sz != 1)
		{
			n.resize(sz - 1);
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

	std::size_t log2i() const
	{
		std::size_t res = (n.size() - 1) * BSZ;
		return res + ::log2i(n.back());
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
};

#endif
