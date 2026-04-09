#ifndef H_FL_LZ4
#define H_FL_LZ4

#include "../data/compr/lz4.h"

namespace fl_pr
{
	class F_lz4
	{
		byteReader* br;
	public:
		bool read(byteReader* r)
		{
			br = r;
			return true;
		}

		bool GetData(byteWriter &bw)
		{
			br->set_pos(0);

			std::vector<uint8_t> out;
			while(br->get_rsize() != 0)
			{
				{
					uint_fast32_t num;
					if (!br->readC<endianness::LITTLE_ENDIAN>(4, num))
						return false;
					if ((num & 0xfffffff0) == 0x184d2a50)
					{
						uint_fast32_t sz;
						if (!br->readC<endianness::LITTLE_ENDIAN>(4, sz))
							return false;
						if (!br->skip(sz))
							return false;
						continue;
					}
					if (num != 0x184d2204)
						return false;
				}

				bool bcs, ccs;
				{
					uint8_t f;
					if (!br->get(f))
						return false;
					const uint_fast8_t sk = 2 + (f & 8) + 4*(f & 1);
					if (!br->skip(sk))
						return false;
					bcs = (f & 16) != 0;
					ccs = (f & 4) != 0;
				}

				for (;;)
				{
					uint_fast32_t bsz;
					if (!br->readC<endianness::LITTLE_ENDIAN>(4, bsz))
						return false;
					if (bsz == 0)
						break;
					if ((bsz & 0x80000000) == 0)
					{
						if (!br->set_rsize(bsz))
							return false;
						if (!compr::lz4::decode_block(*br, out, bw))
							return false;
						br->reset_size();
					}
					else
					{
						bsz &= 0x7fffffff;
						if (!br->readN(out, bsz))
							return false;
					}
					if (bcs)
					{
						if (!br->skip(4))
							return false;
					}
				}
				if (ccs)
				{
					if (!br->skip(4))
						return false;
				}
			}
			bw.writeN(out.data(), out.size());
			bw.Fin();
			return true;
		}
	};
}

#endif
