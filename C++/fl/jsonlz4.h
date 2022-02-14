namespace fl_pr
{
	namespace F_jsonlz4
	{
		const uint_fast8_t offset = 12;
		bool read_size(byteReader &s, uint_fast32_t &fsz)
		{
			uint8_t hdr[8];
			if(!s.readN(hdr, 8))
				return false;
			if(std::memcmp(hdr, "\x6d\x6f\x7a\x4c\x7a\x34\x30\x00", 8) != 0)
				return false;
			if (!s.readC<4, endianness::LITTLE_ENDIAN>(fsz))
				return false;
			return true;
		}
		bool Unpack(byteReader &br, std::vector<uint8_t> &out)
		{
			uint_fast32_t fsize;
			if( !read_size(br, fsize) )
				return false;
			out.clear();
			out.reserve(fsize);
			return convert::lz4::Decoder::Decode_block(br, out);
		}
	}
}
