#ifndef H_BZIP2
#define H_BZIP2

#include <memory>

#include "../htree.h"
#include "../hash/crc32.h"
#include "../../math/base/math_.h"
#include "../byte_writer.h"

namespace convert
{
	namespace BWT
	{
		template<typename T>
		static void Decode(T* res, std::size_t sz, std::size_t p)
		{
			std::vector<std::pair<T, std::size_t>> t(sz);
			for(std::size_t i = 0; i < sz; i++)
			{
				t[i].first = res[i];
				t[i].second = i;
			}
			std::stable_sort(t.begin(), t.end());
			for(std::size_t i = 0; i < sz; i++)
			{
				res[i] = t[p].first;
				p = t[p].second;
			}
		}
	}

	namespace MTF
	{
		void Decode(std::vector<uint8_t> &res, uint8_t* m)
		{
			for(std::size_t i = 0; i < res.size(); i++)
			{
				const auto c = res[i];
				res[i] = m[c];
				std::rotate(m, m + c, m + c + 1);
			}
		}
	}
}

namespace compr
{
	class bzip2
	{
		static std::vector<uint8_t> read_map(bitReaderR &br)
		{
			std::vector<uint8_t> res;
			uint_fast16_t u;
			if( !br.readBE(16, u) )
				return res;
			for(uint_fast8_t i = 0; i < 16; i++)
			{
				if( (u & (0x8000 >> i)) == 0 )
					continue;
				uint_fast16_t c;
				if( !br.readBE(16, c) )
				{
					res.clear();
					return res;
				}
				for (uint_fast8_t j = 0; j < 16; j++)
				{
					if( (c & (0x8000 >> j)) == 0 )
						continue;
					res.push_back((i<<4) + j);
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

			std::vector<uint8_t> p(ntrees);
			for(uint_fast8_t i = 0; i < ntrees; i++)
			{
				p[i] = i;
			}
			convert::MTF::Decode(sel, p.data());
			return true;
		}

		static bool read_trees(bitReaderR &br, uint_fast16_t tsz, std::vector<std::vector<uint8_t>> &trs)
		{
			for(uint_fast8_t i = 0; i < trs.size(); i++)
			{
				trs[i].resize(tsz);

				uint_fast8_t bl;
				if( !br.readBE(5, bl) )
					return false;
				for(uint_fast16_t j = 0; j < tsz; j++)
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
					trs[i][j] = bl;
				}
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
			std::vector<std::unique_ptr<Tree>> htrs(tsz);
			{
				std::vector<std::vector<uint8_t>> trees(tsz);
				if( !read_trees(br, sztr, trees) )
					return false;
				for(uint_fast8_t i = 0; i < tsz; i++)
				{
					htrs[i] = std::unique_ptr<Tree>(new Tree(trees[i].data(), trees[i].size()));
				}
			}

			//huffman + RLE2
			std::size_t sz0 = 0;
			std::size_t l = 0;
			for(uint_fast16_t i = 0; ; i++)
			{
				const auto tr = htrs[sel[i]].get();
				for(uint_fast8_t n = 0; n < 50; n++)
				{
					uint_fast16_t c;
					if( !tr->find(br, c) )
						return false;
					if(c == sztr - 1)
					{
						if(sz0 != 0)
						{
							res.resize(res.size() + sz0);
						}
						return true;
					}
					if(c > 1)
					{
						if(sz0 != 0)
						{
							res.resize(res.size() + sz0);
							sz0 = 0;
							l = 0;
						}
						res.push_back(c - 1);
					}
					else
					{
						sz0 += static_cast<std::size_t>(c + 1) << l;
						l++;
					}
				}
			}
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
		static bool Decode(byteReader &b, byteWriter &bw)
		{
			{
				uint8_t hdr[4];
				if( !b.readN(hdr, 4) )
					return false;
				if(std::memcmp(hdr, "\x42\x5a\x68", 3) != 0)
					return false;
				const uint8_t bsz = hdr[3];
				if(bsz < '1' || bsz > '9')
					return false;
			}
			bitReaderR br(b);

			uint_fast32_t CRC = 0;
			for(;;)
			{
				uint_fast64_t hdr;
				if( !br.readBE(48, hdr) )
					return false;
				if(hdr == 0x314159265359)
				{
					uint_fast32_t bCRC;
					if( !br.readBE(32, bCRC) )
						return false;
					{
						uint_fast8_t rnd;
						if( !br.get(rnd) )
							return false;
						if(rnd == 1)
							return false;
					}

					uint_fast32_t optr;
					if( !br.readBE(24, optr) )
						return false;

					auto smp = read_map(br);
					if(smp.size() == 0)
						return false;

					std::vector<uint8_t> res;
					if( !unpack(br, smp.size() + 2, res) )
						return false;

					convert::MTF::Decode(res, smp.data());
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
					if( !br.readBE(32, sCRC) )
						return false;
					if(sCRC != CRC)
						return false;
					break;
				}
				else
					return false;
			}
			return true;
		}
	};
}

#endif
