#ifndef H_ETH
#define H_ETH

#include <vector>

#include "../data/pack.h"

namespace net
{
	struct Eth
	{
		enum
		{
			IPv4 = 0x0800,
			ARP  = 0x0806,
			IPv6 = 0x86dd
		};

		uint8_t  src[6];
		uint8_t  dst[6];
		uint_fast16_t type;

		bool parse(std::vector<uint8_t> &v)
		{
			if(v.size() < 14)
				return false;
			std::copy_n(v.data(), 6, src);
			std::copy_n(v.data() + 6, 6, dst);
			type = bconv<1, 2, endianness::BIG_ENDIAN>::pack(v.data() + 12);
			v.erase(v.begin(), v.begin() + 14);
			return true;
		}
	};

	namespace IP
	{
		enum
		{
			ICMP   = 0x01,
			TCP    = 0x06,
			UDP    = 0x11,
			RDP    = 0x1b,
			ICMPv6 = 0x3a,
			HIP    = 0x8b
		};
	}

	struct IPv4
	{
		uint8_t src[4];
		uint8_t dst[4];
		uint_fast16_t ID;
		uint_fast8_t type;

		bool parse(std::vector<uint8_t> &v)
		{
			if(v.size() < 20)
				return false;
			const uint_fast8_t s = (v[0] & 0x0f) << 2;
			if(v.size() < s || s < 20)
				return false;
			std::copy_n(v.data() + 12, 4, src);
			std::copy_n(v.data() + 16, 4, dst);
			ID = bconv<1, 2, endianness::BIG_ENDIAN>::pack(v.data() + 4);
			type = v[9];
			v.erase(v.begin(), v.begin() + s);
			return true;
		}
	};

	struct IPv6
	{
		uint8_t src[16];
		uint8_t dst[16];
		uint_fast8_t type;

		bool parse(std::vector<uint8_t> &v)
		{
			if(v.size() < 40)
				return false;
			type = v[6];
			std::copy_n(v.data() +  8, 16, src);
			std::copy_n(v.data() + 24, 16, dst);
			v.erase(v.begin(), v.begin() + 40);
			return true;
		}
	};

	struct TCP
	{
		uint_fast16_t src;
		uint_fast16_t dst;
		uint_fast32_t num;
		uint_fast32_t numAck;
		uint_fast8_t flag;

		enum ftype
		{
			FIN = 0x01,
			SYN = 0x02,
			RST = 0x04,
			PSH = 0x08,
			ACK = 0x10,
		};

		bool parse(std::vector<uint8_t> &v)
		{
			if(v.size() < 13)
				return false;
			const uint_fast8_t s = (v[12] >> 4) << 2;
			if(v.size() < s || s < 20)
				return false;
			src = bconv<1, 2, endianness::BIG_ENDIAN>::pack(v.data());
			dst = bconv<1, 2, endianness::BIG_ENDIAN>::pack(v.data() + 2);
			num = bconv<1, 4, endianness::BIG_ENDIAN>::pack(v.data() + 4);
			numAck = bconv<1, 4, endianness::BIG_ENDIAN>::pack(v.data() + 8);
			flag = v[13];
			v.erase(v.begin(), v.begin() + s);
			return true;
		}

		bool check_type(uint_fast8_t t)
		{
			return (flag & t) != 0;
		}
	};

	struct UDP
	{
		uint_fast16_t src;
		uint_fast16_t dst;

		bool parse(std::vector<uint8_t> &v)
		{
			if(v.size() < 8)
				return false;
			src = bconv<1, 2, endianness::BIG_ENDIAN>::pack(v.data());
			dst = bconv<1, 2, endianness::BIG_ENDIAN>::pack(v.data() + 2);
			v.erase(v.begin(), v.begin() + 8);
			return true;
		}
	};
}

#endif
