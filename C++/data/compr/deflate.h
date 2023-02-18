#ifndef H_DEFLATE
#define H_DEFLATE

#include "../htree.h"
#include "./decompr.h"
#include "../byte_writer.h"

namespace compr
{
	class deflate
	{
		static const std::size_t wsz = 32768;

		static bool fixedH_code(bitReaderL &brd, uint_fast16_t &fc)
		{
			if( !brd.readLE(7, fc) )
				return false;
			if(fc < 24)
			{
				fc += 256;
				return true;
			}
			{
				uint_fast8_t c;
				if( !brd.get(c) )
					return false;
				fc = (fc<<1) | c;
			}
			if(fc < 200)
			{
				if(fc < 192)
				{
					fc -= 48;
				}
				else
				{
					fc += 88;
				}
				return true;
			}
			{
				uint_fast8_t c;
				if( !brd.get(c) )
					return false;
				fc = (fc<<1) | c;
			}
			fc -= 256;
			return true;
		}

		static bool get_size(const uint_fast8_t c, bitReaderL &brd, uint_fast16_t &sz)
		{
			if(c >= 30)
				return false;
			static const uint_fast16_t lengthT[30] = {0,3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,35,43,51,59,67,83,99,115,131,163,195,227,258};
			static const uint_fast8_t length_exT[30] = {0,0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0};

			uint_fast8_t exb;
			if( !brd.readBE(length_exT[c], exb) )
				return false;
			sz = lengthT[c] + exb;
			return true;
		}

		static bool get_dist(const uint_fast8_t c, bitReaderL &brd, uint_fast16_t &dist)
		{
			if(c >= 30)
				return false;
			static const uint_fast16_t distT[30] = {1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577};
			static const uint_fast8_t dist_exT[30] = {0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};

			uint_fast16_t exb;
			if( !brd.readBE(dist_exT[c], exb) )
				return false;
			dist = distT[c] + exb;
			return true;
		}

		static bool decode(const binTree<uint_fast8_t> &codes, uint_fast16_t ncode, bitReaderL &brd, std::vector<uint_fast8_t> &vcodes)
		{
			vcodes.resize(ncode);
			uint_fast16_t n = 0;
			uint_fast8_t o = 0;
			while(n < ncode)
			{
				uint_fast8_t tmp;
				if( !codes.find(brd, tmp) )
					return false;
				if(tmp < 16)
				{
					vcodes[n] = tmp;
					o = tmp;
					n++;
					continue;
				}
				if(tmp == 16)
				{
					uint_fast8_t k;
					if( !brd.readBE(2, k) )
						return false;
					k += 3;
					if(n + k > ncode)
						return false;
					std::fill(vcodes.begin() + n, vcodes.begin() + n + k, o);
					n += k;
					continue;
				}
				o = 0;
				if(tmp == 17)
				{
					uint_fast8_t k;
					if( !brd.readBE(3, k) )
						return false;
					n += k + 3;
				}
				else if(tmp == 18)
				{
					uint_fast8_t k;
					if( !brd.readBE(7, k) )
						return false;
					n += k + 11;
				}
			}
			return true;
		}

		static bool inflate_nocompr(bitReaderL &brd, std::vector<uint8_t> &out)
		{
			uint_fast16_t sz, nsz;
			if( !brd.readB_C<2, endianness::LITTLE_ENDIAN>(sz) )
				return false;
			if( !brd.readB_C<2, endianness::LITTLE_ENDIAN>(nsz) )
				return false;
			if(sz + nsz != 0xffff)
				return false;
			return brd.addB(out, sz);
		}

		static bool inflate_fixed(bitReaderL &brd, std::vector<uint8_t> &out)
		{
			for(;;)
			{
				uint_fast16_t c;
				if( !fixedH_code(brd, c) )
					break;
				if(c < 256)
				{
					out.push_back(c);
					continue;
				}
				if(c == 256)
					return true;

				uint_fast16_t sz, dist;
				uint_fast8_t d = c & 0xff;
				if( !get_size(d, brd, sz) )
					break;
				if( !brd.readLE(5, d) )
					break;
				if( !get_dist(d, brd, dist) )
					break;
				if( !LZ77_repeat(sz, dist, out) )
					break;
			}
			return false;
		}

		static bool inflate_dynamic(bitReaderL &brd, std::vector<uint8_t> &out)
		{
			uint_fast16_t hdr;
			if( !brd.readBE(14, hdr) )
				return false;
			const uint_fast16_t HLIT = (hdr & 0x1f) + 257;
			const uint_fast8_t HDIST = ((hdr >> 5) & 0x1f) + 1;
			const uint_fast8_t HCLEN = (hdr >> 10) + 4;

			const uint_fast8_t csz = 19;
			static const uint_fast8_t co[csz] = {16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15};
			uint_fast8_t clen[csz] = {};
			{
				uint_fast64_t h;
				if( !brd.readBE(3*HCLEN, h) )
					return false;
				for(uint_fast8_t i = 0; h != 0; i++)
				{
					clen[co[i]] = h & 7;
					h >>= 3;
				}
			}

			binTree<uint_fast8_t> codes = makeHTree(clen, csz);
			std::vector<uint_fast8_t> vcodes;
			if( !decode(codes, HLIT + HDIST, brd, vcodes) )
				return false;
			binTree<uint_fast16_t> hlit = makeHTree(vcodes.data(), HLIT);
			binTree<uint_fast8_t> hdist = makeHTree(vcodes.data() + HLIT, HDIST);

			for(;;)
			{
				uint_fast16_t c;
				if( !hlit.find(brd, c) )
					break;
				if(c < 256)
				{
					out.push_back(c);
					continue;
				}
				if(c == 256)
					return true;

				uint_fast16_t sz, dist;
				uint_fast8_t d = c & 0xff;
				if( !get_size(d, brd, sz) )
					break;
				if( !hdist.find(brd, d) )
					break;
				if( !get_dist(d, brd, dist) )
					break;
				if( !LZ77_repeat(sz, dist, out) )
					break;
			}
			return false;
		}
	public:
		static bool Decode(byteReader &br, byteWriter &bw)
		{
			std::vector<uint8_t> out;
			bitReaderL brd(br);
			for(;;)
			{
				uint_fast8_t type;
				if( !brd.readBE(3, type) )
					return false;
				const bool isFin = (type & 1) != 0;
				type >>= 1;
				switch(type)
				{
				case 0:
					if( !inflate_nocompr(brd, out) )
						return false;
					break;
				case 1:
					if( !inflate_fixed(brd, out) )
						return false;
					break;
				case 2:
					if( !inflate_dynamic(brd, out) )
						return false;
					break;
				default:
					return false;
				}
				if(isFin)
					break;
				if(out.size() > wsz)
				{
					bw.writeN(out.data(), out.size() - wsz);
					out.erase(out.begin(), out.end() - wsz);
				}
			}
			bw.writeN(out.data(), out.size());
			bw.Fin();
			return true;
		}
	};
}

#endif
