#ifndef H_PACK
#define H_PACK

#include <cstdint>
#include <cstring>
#include <algorithm>

namespace endianness
{
	enum
	{
		LITTLE_ENDIAN,
		BIG_ENDIAN
	};
}

template<unsigned char SZ> struct UINT_{};
template<> struct UINT_<1>
{
	typedef uint8_t uint;
	typedef uint_fast8_t uint_f;
};
template<> struct UINT_<2>
{
	typedef uint16_t uint;
	typedef uint_fast16_t uint_f;
};
template<> struct UINT_<4>
{
	typedef uint32_t uint;
	typedef uint_fast32_t uint_f;
};
template<> struct UINT_<8>
{
	typedef uint64_t uint;
	typedef uint_fast64_t uint_f;
};

template <unsigned char SZ, char E>
struct bconv {};

template <unsigned char SZ>
struct bconv<SZ, endianness::LITTLE_ENDIAN>
{
	template<typename T>
	static void pack(const typename UINT_<SZ>::uint* a, uint_fast8_t k, T &c)
	{
		c = 0;
		switch (k)
		{
		case 8:
			c = a[7];
			c <<= SZ * 8;
		case 7:
			c |= a[6];
			c <<= SZ * 8;
		case 6:
			c |= a[5];
			c <<= SZ * 8;
		case 5:
			c |= a[4];
			c <<= SZ * 8;
		case 4:
			c |= a[3];
			c <<= SZ * 8;
		case 3:
			c |= a[2];
			c <<= SZ * 8;
		case 2:
			c |= a[1];
			c <<= SZ * 8;
		case 1:
			c |= a[0];
		default:
			break;
		}
	}

	template<typename T>
	static void unpack(T c, uint_fast8_t k, typename UINT_<SZ>::uint* a)
	{
		switch (k)
		{
		case 8:
			a[k - 8] = static_cast<typename UINT_<SZ>::uint>(c);
			c >>= SZ * 8;
		case 7:
			a[k - 7] = static_cast<typename UINT_<SZ>::uint>(c);
			c >>= SZ * 8;
		case 6:
			a[k - 6] = static_cast<typename UINT_<SZ>::uint>(c);
			c >>= SZ * 8;
		case 5:
			a[k - 5] = static_cast<typename UINT_<SZ>::uint>(c);
			c >>= SZ * 8;
		case 4:
			a[k - 4] = static_cast<typename UINT_<SZ>::uint>(c);
			c >>= SZ * 8;
		case 3:
			a[k - 3] = static_cast<typename UINT_<SZ>::uint>(c);
			c >>= SZ * 8;
		case 2:
			a[k - 2] = static_cast<typename UINT_<SZ>::uint>(c);
			c >>= SZ * 8;
		case 1:
			a[k - 1] = static_cast<typename UINT_<SZ>::uint>(c);
		default:
			break;
		}
	}
};
template <unsigned char SZ>
struct bconv<SZ, endianness::BIG_ENDIAN>
{
	template<typename T>
	static void pack(const typename UINT_<SZ>::uint* a, uint_fast8_t k, T &c)
	{
		c = 0;
		switch (k)
		{
		case 8:
			c = a[k - 8];
			c <<= SZ * 8;
		case 7:
			c |= a[k - 7];
			c <<= SZ * 8;
		case 6:
			c |= a[k - 6];
			c <<= SZ * 8;
		case 5:
			c |= a[k - 5];
			c <<= SZ * 8;
		case 4:
			c |= a[k - 4];
			c <<= SZ * 8;
		case 3:
			c |= a[k - 3];
			c <<= SZ * 8;
		case 2:
			c |= a[k - 2];
			c <<= SZ * 8;
		case 1:
			c |= a[k - 1];
		default:
			break;
		}
	}

	template<typename T>
	static void unpack(T c, uint_fast8_t k, typename UINT_<SZ>::uint* a)
	{
		switch (k)
		{
		case 8:
			a[7] = static_cast<typename UINT_<SZ>::uint>(c);
			c >>= SZ * 8;
		case 7:
			a[6] = static_cast<typename UINT_<SZ>::uint>(c);
			c >>= SZ * 8;
		case 6:
			a[5] = static_cast<typename UINT_<SZ>::uint>(c);
			c >>= SZ * 8;
		case 5:
			a[4] = static_cast<typename UINT_<SZ>::uint>(c);
			c >>= SZ * 8;
		case 4:
			a[3] = static_cast<typename UINT_<SZ>::uint>(c);
			c >>= SZ * 8;
		case 3:
			a[2] = static_cast<typename UINT_<SZ>::uint>(c);
			c >>= SZ * 8;
		case 2:
			a[1] = static_cast<typename UINT_<SZ>::uint>(c);
			c >>= SZ * 8;
		case 1:
			a[0] = static_cast<typename UINT_<SZ>::uint>(c);
		default:
			break;
		}
	}
};

namespace conv
{
	template<unsigned char SZ, char E>
	void pack(const uint8_t* a, const std::size_t n, typename UINT_<SZ>::uint* r)
	{
		for(std::size_t i = 0; i < n / SZ; i++)
		{
			bconv<1, E>::pack(a + i*SZ, SZ, r[i]);
		}
	}
	template<unsigned char SZ, char E>
	void unpack(const typename UINT_<SZ>::uint* a, std::size_t n, uint8_t* r)
	{
		for(std::size_t i = 0; i < n; i++)
		{
			bconv<1, E>::unpack(a[i], SZ, r + i*SZ);
		}
	}
}

#endif
