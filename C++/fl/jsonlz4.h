namespace fl_pr
{
	namespace F_jsonlz4
	{
		const uint_fast8_t offset = 12;
		uint32_t read_size(byteReader &s)
		{
			{
				uint8_t hdr[8];
				s.read(hdr, 8);
				if(std::memcmp(hdr, "\x6d\x6f\x7a\x4c\x7a\x34\x30\x00", 8) != 0)
					return 0;
			}
			uint32_t fsize = 0;
			s.getC<endianness::LITTLE_ENDIAN>(fsize);
			return fsize;
		}
		void Unpack(const char* f_in, const char* f_out)
		{
			br_fstream br(f_in);
			auto size = read_size(br);
			if(size == 0)
				return;
			auto data = convert::lz4::Decoder::Decode_block(br, size);
			fwt(f_out, data.data(), data.size());
		}
	}
}
