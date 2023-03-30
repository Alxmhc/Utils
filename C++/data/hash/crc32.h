#ifndef H_CRC32
#define H_CRC32

#include "./hash.h"
#include "../pack.h"

namespace hash
{
	//rfc 1952
	class CRC32 : public HASH<4, 1>
	{
		uint32_t tbl[256];
		uint32_t crc;

		void Init()
		{
			crc = 0xffffffff;
		}
	public:
		static void InitTable(uint32_t* tbl, const uint_fast32_t I)
		{
			uint8_t n = 0;
			do {
				n--;
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
			} while(n != 0);
		}

		CRC32()
		{
			InitTable(tbl, 0xedb88320);
			Init();
		}
		void Update(const uint8_t* v, const std::size_t n)
		{
			for(std::size_t i = 0; i < n; i++)
			{
				crc = tbl[(crc & 0xff) ^ v[i]] ^ (crc >> 8);
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
