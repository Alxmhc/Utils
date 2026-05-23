#ifndef H_ZSTD
#define H_ZSTD

#include "../../math/base/math_.h"
#include "../byte_reader.h"

namespace compr
{
	class zstd
	{
		static bool Init(byteReader &br, bool &ch, std::size_t &dict_sz, uint_fast64_t &fr_sz)
		{
			uint8_t hd;
			if (!br.get(hd))
				return false;

			ch = (hd & 4) != 0;

			dict_sz = 0;
			const bool s_sg = (hd & 0x20) != 0;
			if (!s_sg)
			{
				uint8_t wd;
				if (!br.get(wd))
					return false;
				dict_sz = (wd & 7) + 8;
				dict_sz <<= (wd >> 3) + 7;
			}

			const uint_fast8_t dsz = hd & 3;
			if (!br.skip(dsz == 3 ? 4 : dsz))
				return false;

			fr_sz = 0;
			const uint_fast8_t fsz = hd >> 6;
			if (fsz != 0 || s_sg)
			{
				if (!br.readC<endianness::LITTLE_ENDIAN>(1 << fsz, fr_sz))
					return false;
				if (fsz == 1)
				{
					fr_sz += 256;
				}
			}
			return true;
		}

		static bool read_lit_size(byteReader &br, uint_fast8_t &type, bool &s_str, uint_fast32_t &r_sz, uint_fast32_t &c_sz)
		{
			uint_fast8_t t;
			if (!br.get(t))
				return false;
			type = t & 3;

			if (type < 2)
			{
				c_sz = 0;
				s_str = true;
				if ((t & 4) == 0)
				{
					r_sz = t >> 3;
				}
				else
				{
					if (!br.readC<endianness::LITTLE_ENDIAN>((t & 8) == 0 ? 1 : 2, r_sz))
						return false;
					r_sz = (r_sz << 4) | (t >> 4);
				}
				return true;
			}

			uint_fast8_t sz_f = (t >> 2) & 3;
			s_str = sz_f == 0;
			if (s_str)
			{
				sz_f++;
			}

			uint_fast64_t k;
			if (!br.readC<endianness::LITTLE_ENDIAN>(sz_f + 1, k))
				return false;
			k = (k << 4) | (t >> 4);

			const uint_fast8_t e = 6 + 4 * sz_f;
			r_sz = static_cast<uint_fast32_t>(getBits(k, e));
			c_sz = static_cast<uint_fast32_t>(k >> e);

			return true;
		}
	};
}

#endif
