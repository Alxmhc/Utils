namespace hash
{
	//rfc 1952
	class CRC32
	{
	public:
		static const uint_fast8_t hash_size = 4;

		static void InitTable(uint32_t *tbl, const uint_fast32_t I)
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
	private:
		uint32_t tbl[256];
		uint32_t crc;
	public:
		CRC32() : crc(0xffffffff)
		{
			InitTable(tbl, 0xedb88320);
		}

		void Update(const uint8_t *v, const size_t n)
		{
			for(size_t i = 0; i < n; i++)
			{
				crc = tbl[(crc & 0xff) ^ v[i]] ^ (crc >> 8);
			}
		}

		void Final(uint8_t *r)
		{
			bconv<4, endianness::LITTLE_ENDIAN>::unpack(~crc,  r);
			crc = 0xffffffff;
		}
	};
}
