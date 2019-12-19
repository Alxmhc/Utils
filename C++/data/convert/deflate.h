namespace convert
{
	namespace deflate
	{
		class Decoder
		{
			bitReaderL brd;

			std::vector<uint8_t> inflate_nocompr()
			{
				uint16_t sz;
				brd.getB<endianness::LITTLE_ENDIAN>(sz);
				brd.skipB(2);
				return brd.readB(sz);
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

		public:
			Decoder(std::istream &s) : brd(s) {}
		};
	}
}
