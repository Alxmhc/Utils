#ifndef H_IDEA
#define H_IDEA

#include "../../math/base/math_.h"

namespace crypt
{
	//key sz = 16
	class IDEA
	{
		static void Init_Enc(const uint8_t* k, uint16_t* key)
		{
			auto c = bconv<1, 16, endianness::BIG_ENDIAN>::pack(k);
			for(uint_fast8_t i = 0; i < 6; i++)
			{
				bconv<2, 8, endianness::BIG_ENDIAN>::unpack(c, key + 8*i);
				c = rotl(c, 25);
			}
			bconv<2, 4, endianness::BIG_ENDIAN>::unpack(c.getH(), key + 48);
		}

		static void Init_Dec(const uint8_t* k, uint16_t* key)
		{
			Init_Enc(k, key);
			uint16_t t[52];

			for (uint_fast8_t i = 0; i < 48; i += 6)
			{
				t[i] = mulI(key[48-i]);
				t[i+1] = -key[50-i];
				t[i+2] = -key[49-i];
				t[i+3] = mulI(key[51-i]);
				t[i+4] = key[46-i];
				t[i+5] = key[47-i];
			}
			std::swap(t[1], t[2]);
			t[48] = mulI(key[0]);
			t[49] = -key[1];
			t[50] = -key[2];
			t[51] = mulI(key[3]);

			std::copy_n(t, 52, key);
		}

		static uint16_t mul(uint16_t a, uint16_t b)
		{
			uint_fast32_t t = a;
			t *= b;
			if(t == 0)
				return 1 - a - b;
			return (t % 65537) & 0xffff;
		}
		static uint16_t mulI(uint16_t x)
		{
			return pw_m(x, 0xffff, 0x10001);
		}

		static void Process(const uint16_t* key, uint8_t* r)
		{
			uint16_t d[4];
			conv::pack<2, endianness::BIG_ENDIAN>(r, 8, d);
			for(uint_fast8_t i = 0; i < 48; i += 6)
			{
				uint16_t A = mul(d[0], key[i]);
				uint16_t B = d[1] + key[i + 1];
				uint16_t C = d[2] + key[i + 2];
				uint16_t D = mul(d[3], key[i + 3]);
				uint16_t E = mul(A ^ C, key[i + 4]);
				uint16_t F = mul((B ^ D) + E, key[i + 5]);
				E += F;
				d[0] = A ^ F;
				d[1] = C ^ F;
				d[2] = B ^ E;
				d[3] = D ^ E;
			}
			std::swap(d[1], d[2]);
			d[0] = mul(d[0], key[48]);
			d[1] += key[49];
			d[2] += key[50];
			d[3] = mul(d[3], key[51]);
			conv::unpack<2, endianness::BIG_ENDIAN>(d, 4, r);
		}

	public:
		static const uint_fast8_t block_size = 8;

		class Enc
		{
			uint16_t key[52];
		public:
			static const uint_fast8_t block_size = 8;

			Enc(const uint8_t* k)
			{
				Init_Enc(k, key);
			}

			void process(uint8_t* r) const
			{
				Process(key, r);
			}
		};

		class Dec
		{
			uint16_t key[52];
		public:
			static const uint_fast8_t block_size = 8;

			Dec(const uint8_t* k)
			{
				Init_Dec(k, key);
			}

			void process(uint8_t* r) const
			{
				Process(key, r);
			}
		};
	};
}

#endif
