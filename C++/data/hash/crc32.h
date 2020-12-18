namespace hash
{
	//rfc 1952
	class CRC32
	{
	public:
		static const uint_fast8_t hash_size = 4;
	private:
		uint32_t tbl[256];
		uint32_t crc;
	public:
		CRC32() : crc(0xffffffff)
		{
			uint8_t n = 0;
			do {
				n--;
				uint32_t c = n;
				for (uint_fast8_t k = 0; k < 8; k++)
				{
					if ((c & 1) != 0)
					{
						c = (c >> 1) ^ 0xedb88320;
					}
					else
					{
						c >>= 1;
					}
				}
				tbl[n] = c;
			} while(n != 0);
		}

		void Update(const uint8_t *v, const std::size_t n)
		{
			for(std::size_t i = 0; i < n; i++)
			{
				crc = tbl[(crc & 0xff) ^ v[i]] ^ (crc >> 8);
			}
		}

		void Final(uint8_t *r)
		{
			auto res = ~crc;
			crc = 0xffffffff;
			conv::unpack<endianness::LITTLE_ENDIAN>(res,  r);
		}
	};
}
