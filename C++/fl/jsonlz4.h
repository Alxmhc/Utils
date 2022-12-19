namespace fl_pr
{
	class F_jsonlz4 : public cont_1
	{
		uint_fast32_t fsize;
	public:
		bool read(byteReader* r)
		{
			br = r;

			uint8_t hdr[12];
			if(!br->readN(hdr, 12))
				return false;
			if(std::memcmp(hdr, "\x6d\x6f\x7a\x4c\x7a\x34\x30\x00", 8) != 0)
				return false;
			data_pos = 12;
			data_size = br->get_rsize();
			fsize = bconv<4, endianness::LITTLE_ENDIAN>::pack(hdr + 8);
			return true;
		}

		bool GetData(std::vector<uint8_t> &data)
		{
			std::vector<uint8_t> tmp;
			getData(tmp);
			data.reserve(fsize);
			if( !compr::lz4::Decode(tmp.data(), tmp.size(), data) )
				return false;
			return true;
		}
	};
}
