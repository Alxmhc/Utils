#ifndef H_LZMA
#define H_LZMA

#include "decompr.h"
#include "../byte_reader.h"
#include "../byte_writer.h"

namespace compr
{
	class lzma
	{
		struct props
		{
			uint_fast8_t lc, lp, pb;
			uint32_t dict_size;

			bool Decode_Props(uint_fast8_t p)
			{
				if(p > 224)
					return false;
				lc = p % 9;
				p /= 9;
				lp = p % 5;
				pb = p / 5;
				return true;
			}

			bool Init(const uint8_t* h)
			{
				if( !Decode_Props(h[0]) )
					return false;

				dict_size = bconv<1, 4, endianness::LITTLE_ENDIAN>::pack(h + 1);
				if(dict_size < 4096)
				{
					dict_size = 4096;
				}
				return true;
			}
		};

		class RangeDecoder
		{
			byteReader &br;

			uint32_t Range;
			uint32_t Code;

			bool Normalize()
			{
				if(Range < 0x1000000)
				{
					uint8_t c;
					if( !br.get(c) )
						return false;
					Range <<= 8;
					Code = (Code << 8) | c;
				}
				return true;
			}
		public:
			RangeDecoder(byteReader &brd) : br(brd), Range(0xffffffff) {}

			bool Init()
			{
				uint8_t b;
				if( !br.get(b) )
					return false;
				if( b != 0 )
					return false;

				if( !br.readC<4, endianness::BIG_ENDIAN>(Code) )
					return false;
				if( Code == 0xffffffff )
					return false;

				return true;
			}

			bool DecodeBit(uint16_t &prob, uint_fast8_t &res)
			{
				const auto bound = (Range >> 11) * prob;
				if(Code < bound)
				{
					prob += (2048 - prob) >> 5;
					Range = bound;
					res = 0;
				}
				else
				{
					prob -= prob >> 5;
					Code -= bound;
					Range -= bound;
					res = 1;
				}
				return Normalize();
			}

			bool BitDecode(uint16_t* probs, unsigned char nBits, unsigned int &res)
			{
				res = 1;
				for(unsigned char i = 0; i < nBits; i++)
				{
					uint_fast8_t bit;
					if( !DecodeBit(probs[res], bit) )
						return false;
					res = (res << 1) | bit;
				}
				res -= (unsigned int)1 << nBits;
				return true;
			}

			bool LenDecode(uint16_t* Coder, unsigned char posState, unsigned int &res)
			{
				uint_fast8_t bit;
				if( !DecodeBit(Coder[16*8+16*8+256], bit) )
					return false;
				if(bit == 0)
				{
					if( !BitDecode(Coder + posState*8, 3, res) )
						return false;
					return true;
				}
				if( !DecodeBit(Coder[16*8+16*8+256+1], bit) )
					return false;
				if(bit == 0)
				{
					if( !BitDecode(Coder + 16*8 + posState*8, 3, res) )
						return false;
					res += 8;
					return true;
				}
				if( !BitDecode(Coder + 16*8 + 16*8, 8, res) )
					return false;
				res += 16;
				return true;
			}

			bool DecodeLit(uint16_t* probs, unsigned char state, uint32_t rep, const props &pr, std::vector<uint8_t> &out)
			{
				if( !out.empty() )
				{
					const unsigned int litState = ((out.size() & ((1 << pr.lp) - 1)) << pr.lc) + (out.back() >> (8 - pr.lc));
					probs += litState * 0x300;
				}
				unsigned int symbol = 1;
				if (state >= 7)
				{
					if(rep >= out.size())
						return false;
					uint_fast8_t mByte = out[out.size() - (rep + 1)];
					while (symbol < 0x100)
					{
						uint_fast8_t mBit = (mByte >> 7) & 1;
						mByte <<= 1;
						uint_fast8_t bit;
						if( !DecodeBit(probs[0x100 * (mBit + 1) | symbol], bit) )
							return false;
						symbol = (symbol << 1) | bit;
						if (mBit != bit)
							break;
					}
				}
				while (symbol < 0x100)
				{
					uint_fast8_t bit;
					if( !DecodeBit(probs[symbol], bit) )
						return false;
					symbol = (symbol << 1) | bit;
				}
				out.push_back(symbol & 0xff);
				return true;
			}

			bool DecodeDistance(uint16_t* Pos, unsigned int len, unsigned int &dist)
			{
				if( !BitDecode(Pos + 16 + 115 + (len > 3 ? 3 : len)*64, 6, dist) )
					return false;
				if(dist > 3)
				{
					auto numBits = static_cast<unsigned char>((dist >> 1) - 1);
					auto posSlot = dist;
					dist = 2 | (dist & 1);
					if (posSlot > 13)
					{
						for ( ;numBits > 4; numBits--)
						{
							dist <<= 1;
							Range >>= 1;
							if(Code - Range < 0x80000000)
							{
								Code -= Range;
								dist++;
							}
							if(Code == Range)
								return false;
							if( !Normalize() )
								return false;
						};
						dist <<= numBits;
					}
					else
					{
						dist <<= numBits;
						Pos += 16 + dist - posSlot;
					}
					uint_fast8_t m = 1;
					for(uint_fast8_t i = 0; i < numBits; i++)
					{
						uint_fast8_t bit;
						if( !DecodeBit(Pos[m], bit) )
							return false;
						m = (m << 1) | bit;
						dist |= (unsigned int)bit << i;
					}
				}
				return true;
			}

			bool IsFin() const
			{
				return Code == 0;
			}
		};

		static const unsigned int Len_sz = 2*(512 + 2);
		static const unsigned int Pos_sz = 16 + 115 + 256;
		static const unsigned int Rep_sz = 240 + 192;
	public:
		static bool Decode(byteReader &br, byteWriter &bw, uint64_t fsize = -1)
		{
			props pr;
			{
				uint8_t h[5];
				if( !br.readN(h, 5) )
					return false;
				if( !pr.Init(h) )
					return false;
			}
			if(fsize == static_cast<uint64_t>(-1))
			{
				if( !br.readC<8, endianness::LITTLE_ENDIAN>(fsize) )
					return false;
			}

			RangeDecoder rd(br);
			if( !rd.Init() )
				return false;

			std::vector<uint16_t> probs(Len_sz + Pos_sz + Rep_sz + (0x300 << (pr.lc + pr.lp)), 1024);
			uint16_t* Len = probs.data();
			uint16_t* Pos = Len + Len_sz;
			uint16_t* Rep = Pos + Pos_sz;
			uint16_t* Probs = Rep + Rep_sz;

			std::vector<uint8_t> out;
			if(fsize != static_cast<uint64_t>(-1))
			{
				out.reserve(static_cast<std::size_t>(fsize));
			}

			uint32_t rep0 = 0, rep1 = 0, rep2 = 0, rep3 = 0;
			unsigned char state = 0;

			for(;;)
			{
				if(fsize == 0 && rd.IsFin())
					break;

				unsigned char posState = out.size() & ((1 << pr.pb) - 1);
				const auto state2 = (state << 4) + posState;

				uint_fast8_t bit;
				if( !rd.DecodeBit(Rep[state2 + 240], bit) )
					return false;
				if(bit == 0)
				{
					if (fsize == 0)
						return false;
					if( !rd.DecodeLit(Probs, state, rep0, pr, out) )
						return false;
					state = state < 4 ? 0 : state < 10 ? state - 3 : state - 6;
					fsize--;
					continue;
				}

				if( !rd.DecodeBit(Rep[state], bit) )
					return false;
				unsigned int len;
				if(bit == 0)
				{
					rep3 = rep2;
					rep2 = rep1;
					rep1 = rep0;
					if( !rd.LenDecode(Len, posState, len) )
						return false;
					state = state < 7 ? 7 : 10;
					if( !rd.DecodeDistance(Pos, len, rep0) )
						return false;
					if(rep0 == 0xFFFFFFFF)
					{
						if( !rd.IsFin() )
							return false;
						break;
					}
				}
				else
				{
					if(fsize == 0)
						return false;
					if(out.empty())
						return false;
					if( !rd.DecodeBit(Rep[state + 12], bit) )
						return false;
					if(bit == 0)
					{
						if( !rd.DecodeBit(Rep[state2 + 48], bit) )
							return false;
						if(bit == 0)
						{
							state = state < 7 ? 9 : 11;
							out.push_back(out[out.size() - (rep0 + 1)]);
							fsize--;
							continue;
						}
					}
					else
					{
						if( !rd.DecodeBit(Rep[state + 24], bit) )
							return false;
						if(bit == 0)
						{
							std::swap(rep0, rep1);
						}
						else
						{
							if( !rd.DecodeBit(Rep[state + 36], bit) )
								return false;
							if(bit == 0)
							{
								auto dist = rep2;
								rep2 = rep1;
								rep1 = rep0;
								rep0 = dist;
							}
							else
							{
								auto dist = rep3;
								rep3 = rep2;
								rep2 = rep1;
								rep1 = rep0;
								rep0 = dist;
							}
						}
					}
					if( !rd.LenDecode(Len + Len_sz/2, posState, len) )
						return false;
					state = state < 7 ? 8 : 11;
				}
				len += 2;
				if(fsize < len)
					return false;
				if( !LZ77_repeat(len, rep0 + 1, out) )
					return false;
				fsize -= len;
			}

			bw.writeN(out.data(), out.size());
			bw.Fin();
			return true;
		}
	};
}

#endif
