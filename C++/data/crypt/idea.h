#ifndef H_IDEA
#define H_IDEA

#include "../../math/base/math_.h"
#include "../../math/b_int.h"

namespace crypt
{
	class IDEA
	{
		uint16_t keyEnc[52];
		uint16_t keyDec[52];

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

		class en
		{
			const uint16_t* key;
		public:
			static const uint_fast8_t block_size = 8;
			en(const uint16_t* k) : key(k) {}

			void process(uint8_t* r) const
			{
				Process(key, r);
			}
		};

		class de
		{
			const uint16_t* key;
		public:
			static const uint_fast8_t block_size = 8;
			de(const uint16_t* k) : key(k) {}

			void process(uint8_t* r) const
			{
				Process(key, r);
			}
		};
	public:
		static const uint_fast8_t block_size = 8;

		en Enc;
		de Dec;

		IDEA(const uint8_t* k) : Enc(keyEnc), Dec(keyDec)
		{
			UINT_<16>::uint c = bconv<1, 16, endianness::BIG_ENDIAN>::pack(k);
			for(uint_fast8_t i = 0; i < 6; i++)
			{
				bconv<2, 8, endianness::BIG_ENDIAN>::unpack(c, keyEnc + 8*i);
				c = rotl(c, 25);
			}
			bconv<2, 4, endianness::BIG_ENDIAN>::unpack(c.getH(), keyEnc + 48);

			for (uint_fast8_t i = 0; i < 48; i += 6)
			{
				keyDec[i] = mulI(keyEnc[48-i]);
				keyDec[i+1] = -keyEnc[50-i];
				keyDec[i+2] = -keyEnc[49-i];
				keyDec[i+3] = mulI(keyEnc[51-i]);
				keyDec[i+4] = keyEnc[46-i];
				keyDec[i+5] = keyEnc[47-i];
			}
			std::swap(keyDec[1], keyDec[2]);
			keyDec[48] = mulI(keyEnc[0]);
			keyDec[49] = -keyEnc[1];
			keyDec[50] = -keyEnc[2];
			keyDec[51] = mulI(keyEnc[3]);
		}
	};
}

#endif
