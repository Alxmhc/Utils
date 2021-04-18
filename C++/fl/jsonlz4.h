namespace fl_pr
{
	namespace F_jsonlz4
	{
		const uint_fast8_t offset = 12;
		uint32_t read_size(byteReader &s)
		{
			uint8_t hdr[8];
			if(!s.readN(hdr, 8))
				return 0;
			if(std::memcmp(hdr, "\x6d\x6f\x7a\x4c\x7a\x34\x30\x00", 8) != 0)
				return 0;

			uint8_t sz[4];
			if(!s.readN(sz, 4))
				return 0;
			auto fsize = bconv<4, endianness::LITTLE_ENDIAN>::pack(sz);
			return fsize;
		}
		void Unpack(const char* f_in, const char* f_out)
		{
			br_fstream br(f_in);
			auto size = read_size(br);
			if(size == 0)
				return;
			auto data = convert::lz4::Decoder::Decode_block(br, size);
			bw_fstream fl(f_out);
			fl.write(data.data(), data.size());
		}
	}
}
