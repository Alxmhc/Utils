namespace convert
{
	namespace deflate
	{
		class Decoder
		{
			static uint_fast16_t fixedH_code(bitReaderL &brd)
			{
				uint_fast16_t s = brd.readLE(7);
				if(s < 24)
					return s + 256;
				s = (s<<1) | brd.read1();
				if(s < 200)
				{
					if(s < 192)
						return s - 48;
					return s + 88;
				}
				s = (s<<1) | brd.read1();
				return s - 256;
			}

			static uint_fast16_t get_size(const uint_fast8_t c, bitReaderL &brd)
			{
				static const uint_fast16_t lengthT[30] = {0,3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,35,43,51,59,67,83,99,115,131,163,195,227,258};
				static const uint_fast8_t length_exT[30] = {0,0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0};

				uint_fast16_t sz = lengthT[c];
				uint_fast8_t exb = length_exT[c];
				if(exb != 0)
				{
					sz += brd.readBE(exb);
				}
				return sz;
			}

			static uint_fast16_t get_dist(const uint_fast8_t c, bitReaderL &brd)
			{
				static const uint_fast16_t distT[30] = {1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577};
				static const uint_fast8_t dist_exT[30] = {0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};

				uint_fast16_t dist = distT[c];
				uint_fast8_t exb = dist_exT[c];
				if(exb != 0)
				{
					dist += brd.readBE(exb);
				}
				return dist;
			}

			static std::vector<uint_fast16_t> decode(const huffmanTree<uint_fast8_t> &codes, uint_fast16_t ncode, bitReaderL &brd)
			{
				std::vector<uint_fast16_t> res(ncode);
				uint_fast16_t n = 0, o = 0;
				while(n < ncode)
				{
					uint_fast8_t tmp;
					if( !codes.find(brd, tmp) )
					{
						res.clear();
						break;
					}
					if(tmp < 16)
					{
						res[n] = tmp;
						o = tmp;
						n++;
						continue;
					}
					if(tmp == 16)
					{
						uint_fast8_t k = brd.readBE(2) + 3;
						std::fill(res.begin() + n, res.begin() + n + k, o);
						n += k;
						continue;
					}
					o = 0;
					if(tmp == 17)
					{
						n += brd.readBE(3) + 3;
					}
					else if(tmp == 18)
					{
						n += brd.readBE(7) + 11;
					}
				}
				return res;
			}

			static bool inflate_nocompr(bitReaderL &brd, std::vector<uint8_t> &out)
			{
				uint8_t sza[2];
				if(!brd.readB(sza, 2))
					return false;
				brd.skipB(2);
				auto sz = bconv<2, endianness::LITTLE_ENDIAN>::pack(sza);
				return brd.readB_v(out, sz);
			}

			static bool inflate_fixed(bitReaderL &brd, std::vector<uint8_t> &out)
			{
				for(;;)
				{
					uint_fast16_t c = fixedH_code(brd);
					if(c < 256)
					{
						out.push_back(c);
						continue;
					}
					if(c == 256)
						break;
					uint_fast16_t sz = get_size(c & 0xff, brd);
					c = brd.readLE(5);
					uint_fast16_t dist = get_dist(c, brd);
					if( !LZ77_repeat(sz, dist, out) )
						return false;
				}
				return true;
			}

			static void inflate_dynamic(bitReaderL &brd, std::vector<uint8_t> &out)
			{
				const uint_fast16_t HLIT = brd.readBE(5) + 257;
				const uint_fast8_t HDIST = brd.readBE(5) + 1;
				const uint_fast8_t HCLEN = brd.readBE(4) + 4;

				const uint_fast8_t csz = 19;
				static const uint_fast8_t co[csz] = {16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15};
				uint_fast8_t clen[csz] = {};
				for(uint_fast8_t i = 0; i < HCLEN; i++)
				{
					const uint_fast8_t len = brd.readBE(3);
					if(len == 0)
						continue;
					clen[ co[i] ] = len;
				}
				huffmanTree<uint_fast8_t> codes(clen, csz);

				auto vcodes = decode(codes, HLIT + HDIST, brd);
				huffmanTree<uint_fast16_t> hlit(vcodes.data(), HLIT);
				huffmanTree<uint_fast16_t> hdist(vcodes.data() + HLIT, HDIST);

				uint_fast16_t c;
				while( hlit.find(brd, c) )
				{
					if(c < 256)
					{
						out.push_back(c);
						continue;
					}
					if(c == 256)
						break;

					uint_fast16_t sz = get_size(c & 0xff, brd);
					hdist.find(brd, c);
					uint_fast16_t dist = get_dist(c, brd);
					LZ77_repeat(sz, dist, out);
				}
			}

		public:
			Decoder() {}

			static std::vector<uint8_t> Convert(byteReader &br, size_t nsz = 0)
			{
				bitReaderL brd(br);
				std::vector<uint8_t> out;
				out.reserve(nsz);
				for(;;)
				{
					bool isFin = (brd.read1() == 1);
					uint_fast8_t type = brd.readBE(2);
					switch(type)
					{
					case 0:
						inflate_nocompr(brd, out);
						break;
					case 1:
						inflate_fixed(brd, out);
						break;
					case 2:
						inflate_dynamic(brd, out);
						break;
					default:
						isFin = true;
					}
					if(isFin)
						break;
				}
				return out;
			}
		};
	}
}
