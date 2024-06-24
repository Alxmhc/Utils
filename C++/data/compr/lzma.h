#ifndef H_LZMA
#define H_LZMA

#include <array>

#include "decompr.h"
#include "../byte_reader.h"
#include "../byte_writer.h"

namespace compr
{
	class lzma
	{
		uint_fast8_t lc, lp, pb;
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

		uint32_t dict_size;
		uint64_t fl_size;
		bool Init(byteReader &br)
		{
			uint8_t p;
			if( !br.get(p) )
				return false;
			if( !Decode_Props(p) )
				return false;

			if( !br.readC<4, endianness::LITTLE_ENDIAN>(dict_size) )
				return false;
			if(dict_size < 4096)
			{
				dict_size = 4096;
			}

			if(fl_size == -1)
			{
				if( !br.readC<8, endianness::LITTLE_ENDIAN>(fl_size) )
					return false;
			}

			return true;
		}

		class RangeDecoder
		{
			byteReader* br;

			uint32_t Range;
			uint32_t Code;

			bool Normalize()
			{
				if(Range < 0x1000000)
				{
					uint8_t c;
					if( !br->get(c) )
						return false;
					Range <<= 8;
					Code = (Code << 8) | c;
				}
				return true;
			}
		public:
			bool Init(byteReader &brd)
			{
				br = &brd;

				uint8_t b;
				if( !brd.get(b) )
					return false;
				if( b != 0 )
					return false;

				if( !brd.readC<4, endianness::BIG_ENDIAN>(Code) )
					return false;
				if( Code == 0xffffffff )
					return false;

				Range = 0xffffffff;
				return true;
			}

			bool DecodeDirectBits(unsigned char numBits, unsigned int &res)
			{
				res = 0;
				do{
					res <<= 1;
					Range >>= 1;
					if(Code - Range < 0x80000000)
					{
						Code -= Range;
						res++;
					}
					if(Code == Range)
						return false;
					if( !Normalize() )
						return false;
				} while (--numBits);
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

			bool RevBitDecode(uint16_t* probs, unsigned char nBits, unsigned int &res)
			{
				unsigned int m = 1;
				res = 0;
				for(unsigned char i = 0; i < nBits; i++)
				{
					uint_fast8_t bit;
					if( !DecodeBit(probs[m], bit) )
						return false;
					m = (m << 1) | bit;
					res |= (unsigned int)bit << i;
				}
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

			bool IsFin() const
			{
				return Code == 0;
			}
		};

		std::vector<uint16_t> LitProbs;

		bool DecodeLiteral(unsigned char state, uint32_t rep)
		{
			unsigned int litState = out.empty() ? 0 : ((out.size() & ((1 << lp) - 1)) << lc) + (out.back() >> (8 - lc));
			uint16_t* probs = LitProbs.data() + (unsigned int)0x300 * litState;
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
					if( !rd.DecodeBit(probs[0x100 * (mBit + 1) | symbol], bit) )
						return false;
					symbol = (symbol << 1) | bit;
					if (mBit != bit)
						break;
				}
			}
			while (symbol < 0x100)
			{
				uint_fast8_t bit;
				if( !rd.DecodeBit(probs[symbol], bit) )
					return false;
				symbol = (symbol << 1) | bit;
			}
			out.push_back(symbol & 0xff);
			return true;
		}

		std::array<uint16_t, 2*(16*8 + 16*8 + 256 + 2)> Len;

		std::array<uint16_t, 16 + 115 + 4*64> PosDecoders;

		bool DecodeDistance(unsigned int len, unsigned int &dist)
		{
			if(len > 3)
			{
				len = 3;
			}
			
			if( !rd.BitDecode(PosDecoders.data() + 16 + 115 + len*64, 6, dist) )
				return false;
			if(dist < 4)
				return true;
			
			unsigned char numDirectBits = static_cast<unsigned char>((dist >> 1) - 1);
			unsigned int posSlot = dist;
			dist = ((2 | (posSlot & 1)) << numDirectBits);
			if (posSlot < 14)
			{
				unsigned int d;
				if( !rd.RevBitDecode(PosDecoders.data() + 16 + dist - posSlot, numDirectBits, d) )
					return false;
				dist += d;
			}
			else
			{
				unsigned int d;
				if( !rd.DecodeDirectBits(numDirectBits - 4, d) )
					return false;
				dist += d << 4;
				const unsigned char nbits = 4;
				if( !rd.RevBitDecode(PosDecoders.data(), nbits, d) )
					return false;
				dist += d;
			}
			return true;
		}

		std::array<uint16_t, 240 + 192> mRep;

		RangeDecoder rd;
		std::vector<uint8_t> out;
	public:
		lzma(uint64_t fsz = -1) : fl_size(fsz)
		{
			PosDecoders.fill(1024);
			mRep.fill(1024);
			Len.fill(1024);
		}

		bool Decode(byteReader &br, byteWriter &bw)
		{
			if( !Init(br) )
				return false;
			if( !rd.Init(br) )
				return false;
			LitProbs.resize(0x300 << (lc + lp), 1024);

			uint32_t rep0 = 0, rep1 = 0, rep2 = 0, rep3 = 0;
			unsigned char state = 0;

			auto fsize = fl_size;
			for(;;)
			{
				if(fsize == 0 && rd.IsFin())
					break;

				unsigned char posState = out.size() & ((1 << pb) - 1);
				const auto state2 = (state << 4) + posState;

				uint_fast8_t bit;
				if( !rd.DecodeBit(mRep[state2 + 240], bit) )
					return false;
				if(bit == 0)
				{
					if (fsize == 0)
						return false;
					if( !DecodeLiteral(state, rep0) )
						return false;
					state = state < 4 ? 0 : state < 10 ? state - 3 : state - 6;
					fsize--;
					continue;
				}

				if( !rd.DecodeBit(mRep[state], bit) )
					return false;
				unsigned int len;
				if(bit != 0)
				{
					if(fsize == 0)
						return false;
					if(out.empty())
						return false;
					if( !rd.DecodeBit(mRep[state + 12], bit) )
						return false;
					if(bit == 0)
					{
						if( !rd.DecodeBit(mRep[state2 + 48], bit) )
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
						if( !rd.DecodeBit(mRep[state + 24], bit) )
							return false;
						if(bit == 0)
						{
							std::swap(rep0, rep1);
						}
						else
						{
							if( !rd.DecodeBit(mRep[state + 36], bit) )
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
					if( !rd.LenDecode(Len.data() + Len.size()/2, posState, len) )
						return false;
					state = state < 7 ? 8 : 11;
				}
				else
				{
					rep3 = rep2;
					rep2 = rep1;
					rep1 = rep0;
					if( !rd.LenDecode(Len.data(), posState, len) )
						return false;
					state = state < 7 ? 7 : 10;
					if( !DecodeDistance(len, rep0) )
						return false;
					if(rep0 == 0xFFFFFFFF)
					{
						if( !rd.IsFin() )
							return false;
						break;
					}
				}
				len += 2;
				if(fsize < len)
					return false;
				if( !LZ77_repeat(len, rep0 + 1, out) )
					return false;
				fsize -= len;
			}

			bw.writeN(out.data(), out.size());
			return true;
		}
	};
}

#endif
