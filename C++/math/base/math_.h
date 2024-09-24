#ifndef H_MATH_
#define H_MATH_

#include <cstdint>
#include <cmath>

template <typename B>
B rotl(B x, unsigned char d)
{
	return (x<<d) | (x>>((sizeof(B)<<3)-d));
}

template <typename B>
B rotr(B x, unsigned char d)
{
	return (x>>d) | (x<<((sizeof(B)<<3)-d));
}

template <typename B>
B rotl(B x, unsigned char d, unsigned char sz)
{
	B m(1);
	m = (m << sz) - 1;
	return ((x<<d) | (x>>(sz-d))) & m;
}

template <typename B>
B rotr(B x, unsigned char d, unsigned char sz)
{
	B m(1);
	m = (m << sz) - 1;
	return ((x>>d) | (x<<(sz-d))) & m;
}

template<typename T>
uint_fast8_t log2i(T c)
{
	uint_fast8_t res = 0;
	for(uint_fast8_t p = sizeof(T) * 4; p != 0; p >>= 1)
	{
		const auto t = c >> p;
		if(t == 0)
			continue;
		c = t;
		res |= p;
	}
	return res;
}

//76543210
template<typename T>
static uint_fast8_t getBit(T c, uint_fast8_t n)
{
	return (c >> n) & 1;
}
template<typename T>
static void setBit0(T &c, uint_fast8_t n)
{
	const T m = static_cast<T>(1) << n;
	c &= ~m;
}
template<typename T>
static void setBit1(T &c, uint_fast8_t n)
{
	const T m = static_cast<T>(1) << n;
	c |= m;
}

const long double pi = 3.1415926535897932384626;

template<typename T>
T operator+(const T &a, const T &b)
{
	T r(a);
	r += b;
	return r;
}
template<typename T>
T operator-(const T &a, const T &b)
{
	T r(a);
	r -= b;
	return r;
}

static long double dot(long double a, long double b)
{
	return a*b;
}

namespace vt
{
	template<typename T>
	long double abs(const T &a)
	{
		return std::sqrt(dot(a, a));
	}
	static long double abs(long double a)
	{
		return std::abs(a);
	}

	template<typename T>
	void nrm(T &a)
	{
		a /= abs(a);
	}
}

//(a^b)%c
static uint32_t pw_m(uint64_t a, uint_fast64_t b, uint32_t c)
{
	if(c == 1)
		return 0;
	if(b == 0)
		return 1;
	a %= c;
	if(a < 2 || b == 1)
		return static_cast<uint32_t>(a);
	uint_fast64_t r = 1;
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
	return static_cast<uint32_t>(r);
}

#endif
