namespace fl_pr
{
	class F_jsonlz4
	{
		br_fstream br;

		struct inf
		{
			size_t data_pos;
			size_t data_size;

			uint_fast32_t fsize;
		};
		inf f_inf;

		bool read_inf()
		{
			uint8_t hdr[12];
			if(!br.readN(hdr, 12))
				return false;
			if(std::memcmp(hdr, "\x6d\x6f\x7a\x4c\x7a\x34\x30\x00", 8) != 0)
				return false;
			f_inf.data_pos = 12;
			f_inf.data_size = br.get_rsize();
			f_inf.fsize = bconv<4, endianness::LITTLE_ENDIAN>::pack(hdr + 8);
			return true;
		}

		void getData(std::vector<uint8_t> &data)
		{
			br.set_pos(f_inf.data_pos);
			br.readN(data, f_inf.data_size);
		}
	public:
		bool open(const char* fl)
		{
			if( !br.open(fl) )
				return false;
			if( !read_inf() )
				return false;
			return true;
		}

		bool GetData(std::vector<uint8_t> &data)
		{
			std::vector<uint8_t> tmp;
			getData(tmp);
			data.reserve(f_inf.fsize);
			if( !compr::lz4::Decode(tmp.data(), tmp.size(), data) )
				return false;
			return true;
		}
	};
}
