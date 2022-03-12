namespace fl_pr
{
	class F_jsonlz4
	{
		br_fstream br;
		uint_fast32_t fsz;
	public:
		bool open(const char* fl)
		{
			if( !br.open(fl) )
				return false;
			uint8_t hdr[12];
			if(!br.readN(hdr, 12))
				return false;
			if(std::memcmp(hdr, "\x6d\x6f\x7a\x4c\x7a\x34\x30\x00", 8) != 0)
				return false;
			fsz = bconv<4, endianness::LITTLE_ENDIAN>::pack(hdr + 8);
			return true;
		}
		bool getData(byteWriter &bw)
		{
			br.set_pos(12);
			std::vector<uint8_t> data;
			br.readN(data, br.get_size() - 12);
			bw.writeN(data.data(), data.size());
			return true;
		}
	};
}
