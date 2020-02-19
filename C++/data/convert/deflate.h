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

			static const uint_fast16_t mx_dist = 32768;

			static uint_fast16_t get_dist(const uint_fast16_t c, bitReaderL &brd)
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

			static void repeat(uint_fast16_t sz, uint_fast16_t dist, std::vector<uint8_t> &out)
			{
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

			template<typename T>
			static binTree<T> buildTree(T *m, T n)
			{
				typedef std::pair<uint_fast16_t, uint_fast16_t> pr;
				std::vector<pr> tmp(n);
				T sz = 0;
				for(T i = 0; i < n; ++i)
				{
					if(m[i] == 0)
						continue;
					tmp[sz].first = i;
					tmp[sz].second = m[i];
					sz++;
				}
				std::sort(tmp.begin(), tmp.begin() + sz, [](const pr &a, const pr &b){return a.second == b.second ? a.first < b.first : a.second < b.second;});

				binTree<T> t;
				for(T i = 0, k = 0, p = 0; i < sz; ++i)
				{
					const T s = tmp[i].second;
					if(p != s)
					{
						k <<= s - p;
						p = s;
					}
					t.add(k, s, tmp[i].first);
					k++;
				}
				return t;
			}

			static std::vector<uint_fast16_t> decode(const binTree<uint_fast8_t> &codes, uint_fast16_t ncode, bitReaderL &brd)
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

			static void inflate_nocompr(bitReaderL &brd, std::vector<uint8_t> &out)
			{
				uint16_t sz;
				brd.getB<endianness::LITTLE_ENDIAN>(sz);
				brd.skipB(2);
				const auto end = out.size();
				out.resize(end + sz);
				brd.readB(out.data() + end, sz);
			}

			static void inflate_fixed(bitReaderL &brd, std::vector<uint8_t> &out)
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
					repeat(sz, dist, out);
				}
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
				auto codes = buildTree<uint_fast8_t>(clen, csz);

				auto vcodes = decode(codes, HLIT + HDIST, brd);
				auto hlit = buildTree<uint_fast16_t>(vcodes.data(), HLIT);
				auto hdist = buildTree<uint_fast16_t>(vcodes.data() + HLIT, HDIST);

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
					repeat(sz, dist, out);
				}
			}

		public:
			Decoder() {}

			void Convert(byteReader &br, std::ostream &r) const
			{
				bitReaderL brd(br);
				std::vector<uint8_t> out;
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
					if(out.size() > mx_dist)
					{
						std::size_t t = out.size() - mx_dist;
						r.write(reinterpret_cast<const char*>(out.data()), t);
						out.erase(out.begin(), out.begin() + t);
					}
					if(isFin)
						break;
				}
				r.write(reinterpret_cast<const char*>(out.data()), out.size());
			}
		};
	}
}
