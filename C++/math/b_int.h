#ifndef H_B_INT
#define H_B_INT

#include <vector>
#include <algorithm>

#include "../data/pack.h"

class b_int
{
	typedef uint32_t num;
	typedef uint64_t num2;
	static const uint_fast8_t BSZ = 32;

	std::vector<num> n;

	signed char compare(const b_int &c) const
	{
		auto sz = n.size();
		{
			const auto csz = c.n.size();
			if(sz != csz)
				return sz > csz ? 1 : -1;
		}
		while(sz--)
		{
			if(n[sz] != c.n[sz])
				return n[sz] > c.n[sz] ? 1 : -1;
		}
		return 0;
	}

	void fix()
	{
		auto sz = n.size();
		while(sz != 1 && n[sz-1] == 0)
		{
			sz--;
		}
		n.resize(sz);
	}

	//*this >= c
	const b_int& operator-=(const b_int &c)
	{
		if(c == 0)
			return *this;
		if(this == &c)
		{
			*this = 0;
			return *this;
		}

		std::size_t i = 0;
		while(c.n[i] == 0)
		{
			i++;
		}

		const auto csz = c.n.size();

		bool d = false;
		for(; i < csz; i++)
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
public:
	explicit b_int(num c = 0) : n(1, c) {}

	const b_int& operator=(num a)
	{
		n = std::vector<num>(1, a);
		return *this;
	}

	void fromB(const uint8_t* v, std::size_t k)
	{
		n = std::vector<num>((k+3)>>2);
		std::size_t i = 0;
		while(k > 3)
		{
			k -= 4;
			n[i++] = bconv<1, 4, endianness::BIG_ENDIAN>::pack(v + k);
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

	bool operator==(const b_int &c) const
	{
		return compare(c) == 0;
	}
	bool operator!=(const b_int &c) const
	{
		return !operator==(c);
	}
	bool operator>(const b_int &c) const
	{
		return compare(c) > 0;
	}
	bool operator<(const b_int &c) const
	{
		return compare(c) < 0;
	}
	bool operator>=(const b_int &c) const
	{
		return !operator<(c);
	}

	const b_int& operator>>=(std::size_t c)
	{
		if(*this == 0)
			return *this;

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
		for(std::size_t i = 0;;)
		{
			n[i] >>= c;
			i++;
			if(i == sz)
				break;
			n[i-1] |= n[i] << (BSZ - c);
		}

		fix();
		return *this;
	}

	const b_int& operator<<=(std::size_t c)
	{
		if(*this == 0)
			return *this;

		std::size_t i = 0;
		while(n[i] == 0)
		{
			i++;
		}

		const auto k = c / BSZ;
		if(k != 0)
		{
			i += k;
			std::vector<num> tmp(k);
			n.insert(n.begin(), tmp.cbegin(), tmp.cend());
			c %= BSZ;
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

	const b_int& operator+=(const b_int &c)
	{
		if(c == 0)
			return *this;
		if(this == &c)
			return *this <<= 1;

		const auto csz = c.n.size();
		if(n.size() < csz)
		{
			n.resize(csz);
		}

		std::size_t i = 0;
		while(c.n[i] == 0)
		{
			i++;
		}

		bool d = false;
		for(; i < csz; i++)
		{
			n[i] += c.n[i];
			if(d)
			{
				n[i]++;
				d = (n[i] <= c.n[i]);
			}
			else
			{
				d = (n[i] < c.n[i]);
			}
		}
		while(d)
		{
			if(i == n.size())
			{
				n.push_back(1);
				break;
			}
			n[i]++;
			d = (n[i] == 0);
			i++;
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
			t = t*a + d;
			n[i] = static_cast<num>(t);
			d = t >> BSZ;
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
		if(*this == 0)
			return *this;

		const auto csz = c.n.size();
		if(csz == 1)
		{
			*this *= c.n[0];
			return *this;
		}

		b_int res;
		res.n.resize(n.size() + csz - 1);

		for(std::size_t i = 0;;)
		{
			res += *this * c.n[i];
			i++;
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

	num operator%(num c)
	{
		if(c == 1)
			return 0;
		auto sz = n.size() - 1;
		num2 r = n[sz] % c;
		while(sz--)
		{
			r = ((r << BSZ) | n[sz]) % c;
		}
		return static_cast<num>(r);
	}

	const b_int& operator%=(const b_int &c)
	{
		if(*this < c)
			return *this;
		if(c.n.size() == 1)
		{
			*this = *this % c.n[0];
			return *this;
		}

		std::size_t d = (n.size() - c.n.size()) * BSZ;
		{
			auto f = n.back();
			while(f < c.n.back())
			{
				f <<= 1;
				d--;
			}
			while(f > c.n.back())
			{
				f >>= 1;
				d++;
			}
		}

		b_int t(c);
		t <<= d;
		for(; d != 0; d--)
		{
			t >>= 1;
			if(*this >= t)
			{
				*this -= t;
			}
		}
		
		return *this;
	}

	//(a^b)%c
	template<typename T>
	static T pw_m(T a, num b, const T &c)
	{
		T r(1);
		if(b == 0)
			return r;
		a %= c;
		if(a < 2)
			return a;
		for(;;)
		{
			if((b & 1) != 0)
			{
				r *= a;
				r %= c;
				if(b == 1)
					break;
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

//(a^b)%c
static uint_fast32_t pw_m(uint_fast32_t a, uint_fast32_t b, uint_fast32_t c)
{
	const auto r = b_int::pw_m(uint_fast64_t(a), b, uint_fast64_t(c));
	return static_cast<uint_fast32_t>(r);
}

#endif
