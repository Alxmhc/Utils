#ifndef H_BZIP2
#define H_BZIP2

#include "../convert/conv.h"
#include "../htree.h"
#include "../bin.h"
#include "../hash/crc32.h"
#include "../../math/base/math_.h"
#include "../byte_writer.h"

namespace compr
{
	class bzip2
	{
		static std::vector<uint8_t> read_map(bitReaderR &br)
		{
			std::vector<uint8_t> res;
			res.reserve(256);

			uint_fast16_t a;
			if( !br.readLE(16, a) )
				return res;
			for(uint_fast8_t i = 0; a != 0; i+=16,a>>=1)
			{
				if( (a & 1) == 0 )
					continue;
				uint_fast16_t b;
				if( !br.readLE(16, b) )
				{
					res.clear();
					return res;
				}
				for (uint_fast8_t j = 0; b != 0; j++,b>>=1)
				{
					if( (b & 1) == 0 )
						continue;
					res.push_back(i + j);
				}
			}
			return res;
		}

		static bool read_sel(bitReaderR &br, uint_fast8_t ntrees, std::vector<uint8_t> &sel)
		{
			uint_fast16_t sz;
			if( !br.readBE(15, sz) )
				return false;
			if(sz == 0)
				return false;
			sel.resize(sz);
			for(uint_fast16_t i = 0; i < sz; i++)
			{
				uint_fast8_t c = 0;
				for(;;)
				{
					uint_fast8_t t;
					if( !br.get(t) )
						return false;
					if(t == 0)
						break;
					c++;
					if(c == ntrees)
						return false;
				}
				sel[i] = c;
			}

			convert::MTF::Decode_t(sel.data(), sel.size(), ntrees);
			return true;
		}

		static bool read_tree(bitReaderR &br, std::vector<uint8_t> &tr)
		{
			uint_fast8_t bl;
			if( !br.readBE(5, bl) )
				return false;
			for(uint_fast16_t i = 0; i < tr.size(); i++)
			{
				for(;;)
				{
					uint_fast8_t t;
					if( !br.get(t) )
						return false;
					if(t == 0)
						break;
					if( !br.get(t) )
						return false;
					if(t == 1)
					{
						bl--;
					}
					else
					{
						bl++;
					}
				}
				tr[i] = bl;
			}
			return true;
		}

		static bool unpack(bitReaderR &br, uint_fast16_t sztr, std::vector<uint8_t> &res)
		{
			uint_fast8_t tsz;
			if( !br.readBE(3, tsz) )
				return false;
			if(tsz < 2 || tsz > 6)
				return false;

			//selectors
			std::vector<uint8_t> sel;
			if( !read_sel(br, tsz, sel) )
				return false;

			//construct trees
			typedef hTree<uint_fast16_t> Tree;
			std::vector<Tree> htrs;
			htrs.reserve(tsz);
			{
				std::vector<uint8_t> tree(sztr);
				for(uint_fast8_t i = 0; i < tsz; i++)
				{
					if( !read_tree(br, tree) )
						return false;
					htrs.push_back(Tree(tree.data(), static_cast<uint_fast16_t>(tree.size())));
				}
			}

			//huffman + RLE2
			std::size_t sz0 = 0;
			uint_fast8_t l = 0;
			for(uint_fast16_t i = 0; i < sel.size(); i++)
			{
				const auto tr = &htrs[sel[i]];
				for(uint_fast8_t n = 0; n < 50; n++)
				{
					uint_fast16_t c;
					if( !btree_decode(tr, br, c) )
						return false;
					if(c < 2)
					{
						sz0 += static_cast<std::size_t>(c + 1) << l;
						l++;
						continue;
					}
					if(sz0 != 0)
					{
						res.resize(res.size() + sz0);
						sz0 = 0;
						l = 0;
					}
					if(c == sztr - 1)
						return true;
					res.push_back(c - 1);
				}
			}
			return false;
		}

		static bool RLE1_Decode(std::vector<uint8_t> &res)
		{
			const uint_fast8_t kl = 4 - 1;

			const std::size_t sz = res.size();
			std::size_t nsz = sz;
			std::vector<std::size_t> id;

			std::size_t i = 0;
			while(i + kl < sz)
			{
				uint8_t c = res[i++];
				uint_fast8_t k = 0;
				for(; k < kl; k++)
				{
					if(res[i] != c)
						break;
					i++;
				}
				if(k != kl)
					continue;
				if(i == sz)
					return false;
				nsz--;
				nsz += res[i];
				id.push_back(i);
				i++;
			}
			if(id.empty())
				return true;

			std::vector<uint8_t> tmp(nsz);
			std::size_t pos = 0;
			std::size_t pe = 0;
			for(i = 0; i < id.size(); i++)
			{
				const auto p = id[i];
				std::copy(res.begin()+pos, res.begin()+p, tmp.begin() + pe);
				const auto k = res[p];
				pe += p - pos;
				if(k != 0)
				{
					std::fill_n(tmp.begin() + pe, k, res[p-1]);
					pe += k;
				}
				pos = p + 1;
			}
			std::copy(res.begin()+pos, res.end(), tmp.begin() + pe);
			res = std::move(tmp);
			return true;
		}

		static uint_fast32_t bcrc(const std::vector<uint8_t> &v)
		{
			hash::CRC32 crc32;
			for(std::size_t i = 0; i < v.size(); i++)
			{
				crc32.Update(bitRevT[v[i]]);
			}
			uint8_t hash[hash::CRC32::hash_size];
			crc32.Final(hash);
			for(uint_fast8_t i = 0; i < hash::CRC32::hash_size; i++)
			{
				hash[i] = bitRevT[hash[i]];
			}
			return bconv<1, 4, endianness::BIG_ENDIAN>::pack(hash);
		}
	public:
		static bool Decode(byteReader &br, byteWriter &bw)
		{
			{
				uint8_t hdr[4];
				if( !br.readN(hdr, 4) )
					return false;
				if(std::memcmp(hdr, "\x42\x5a\x68", 3) != 0)
					return false;
				const uint8_t bsz = hdr[3];
				if(bsz < '1' || bsz > '9')
					return false;
			}
			bitReaderR brd(br);

			uint_fast32_t CRC = 0;
			for(;;)
			{
				uint_fast64_t hdr;
				if( !brd.readBE(48, hdr) )
					return false;
				if(hdr == 0x314159265359)
				{
					uint_fast32_t bCRC;
					if( !brd.readBE(32, bCRC) )
						return false;
					{
						uint_fast8_t rnd;
						if( !brd.get(rnd) )
							return false;
						if(rnd == 1)
							return false;
					}

					uint_fast32_t optr;
					if( !brd.readBE(24, optr) )
						return false;

					auto smp = read_map(brd);
					if(smp.size() == 0)
						return false;

					std::vector<uint8_t> res;
					if( !unpack(brd, static_cast<uint_fast16_t>(smp.size() + 2), res) )
						return false;

					convert::MTF::Decode(res.data(), res.size(), smp.data());
					convert::BWT::Decode(res.data(), res.size(), optr);
					RLE1_Decode(res);
					if(bcrc(res) != bCRC)
						return false;

					bw.writeN(res.data(), res.size());
					CRC = rotl(CRC, 1) ^ bCRC;
				}
				else if(hdr == 0x177245385090)
				{
					uint_fast32_t sCRC;
					if( !brd.readBE(32, sCRC) )
						return false;
					if(sCRC != CRC)
						return false;
					break;
				}
				else
					return false;
			}
			bw.Fin();
			return true;
		}
	};
}

#endif
