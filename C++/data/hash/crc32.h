#ifndef H_CRC32
#define H_CRC32

#include "../pack.h"

namespace hash
{
	//rfc 1952
	class CRC32
	{
	public:
		static const uint_fast8_t hash_size = 4;
		static const uint_fast8_t block_size = 1;
	private:
		uint32_t tbl[256];
		uint32_t crc;

		void Init()
		{
			crc = 0xffffffff;
		}
	public:
		static void InitTable(uint32_t* tbl, const uint_fast32_t I)
		{
			tbl[0] = 0;
			for(uint_fast8_t n = 255; n != 0; n--)
			{
				uint32_t c = n;
				for (uint_fast8_t k = 0; k < 8; k++)
				{
					if ((c & 1) != 0)
					{
						c = (c >> 1) ^ I;
					}
					else
					{
						c >>= 1;
					}
				}
				tbl[n] = c;
			}
		}

		CRC32()
		{
			InitTable(tbl, 0xedb88320);
			Init();
		}
		void Update(uint8_t c)
		{
			crc = tbl[(crc & 0xff) ^ c] ^ (crc >> 8);
		}
		void Update(const uint8_t* v, const std::size_t n)
		{
			for(std::size_t i = 0; i < n; i++)
			{
				Update(v[i]);
			}
		}
		void Final(uint8_t* r)
		{
			bconv<1, 4, endianness::LITTLE_ENDIAN>::unpack(~crc,  r);
			Init();
		}
	};
}

#endif
