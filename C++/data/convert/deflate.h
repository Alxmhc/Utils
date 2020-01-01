namespace convert
{
	namespace deflate
	{
		class Decoder
		{
			bitReaderL brd;
			std::vector<uint8_t> out;

			void inflate_nocompr()
			{
				uint16_t sz;
				brd.getB<endianness::LITTLE_ENDIAN>(sz);
				brd.skipB(2);
				brd.readB(sz, out);
			}

			uint_fast16_t fixedH_code()
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

			void inflate_fixed()
			{
				static const uint_fast16_t lengthT[29] = {3,4,5,6,7,8,9,10,11,13,15,17,19,23,27,31,35,43,51,59,67,83,99,115,131,163,195,227,258};
				static const uint_fast8_t length_exT[29] = {0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0};
				static const uint_fast16_t distT[30] = {1,2,3,4,5,7,9,13,17,25,33,49,65,97,129,193,257,385,513,769,1025,1537,2049,3073,4097,6145,8193,12289,16385,24577};
				static const uint_fast8_t dist_exT[30] = {0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};

				for(;;)
				{
					auto c = fixedH_code();
					if(c < 256)
					{
						out.push_back(c);
						continue;
					}
					if(c == 256)
						break;
					c -= 257;

					uint_fast16_t sz, dist;
					{
						sz = lengthT[c];
						uint_fast8_t exb = length_exT[c];
						if(exb != 0)
						{
							sz += brd.readBE(exb);
						}

						c = brd.readLE(5);
						dist = distT[c];
						exb = dist_exT[c];
						if(exb != 0)
						{
							dist += brd.readBE(exb);
						}
					}

					std::size_t osz = out.size();
					out.resize(osz + sz);
					std::size_t n = osz - dist;
					while(sz > dist)
					{
						std::copy_n(out.begin() + n, dist, out.begin() + osz);
						osz += dist;
						sz -= dist;
					}
					std::copy_n(out.begin() + n, sz, out.begin() + osz);
				}
			}

		public:
			Decoder(std::istream &s) : brd(s) {}

			std::vector<uint8_t> Convert()
			{
				for(;;)
				{
					bool isFin = (brd.read1() == 1);
					uint_fast8_t type = brd.readBE(2);
					switch(type)
					{
					case 0:
						inflate_nocompr();
						break;
					case 1:
						inflate_fixed();
						break;
					case 2:
						//TODO: inflate_dynamic();
						break;
					default:
						isFin = true;
					}
					if(isFin)
						break;
				}
				return std::move(out);
			}
		};
	}
}
