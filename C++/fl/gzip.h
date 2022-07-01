namespace fl_pr
{
	class F_gzip
	{
		br_fstream br;

		struct inf
		{
			size_t data_pos;
			size_t data_size;
			std::string name;

			uint_fast32_t fsize;
			uint8_t crc32[4];

			std::string comment;
		};
		inf f_inf;

		bool read_inf()
		{
			uint8_t flg;
			{
				uint8_t hdr[4];
				if( !br.readN(hdr, 4) )
					return false;
				if(std::memcmp(hdr, "\x1f\x8b\x08", 3) != 0)
					return false;
				flg = hdr[3];
			}
			if(flg > 31)
				return false;
			if( !br.skip(6) )
				return false;
			if( (flg & 4) != 0 )
			{
				uint_fast16_t sz;
				if (!br.readC<2, endianness::LITTLE_ENDIAN>(sz))
					return false;
				if (!br.skip(sz))
					return false;
			}
			if( (flg & 8) != 0 )
			{
				 if( !br.read_string(0, f_inf.name) )
					 return false;
			}
			if( (flg & 16) != 0 )
			{
				 if( !br.read_string(0, f_inf.comment) )
					 return false;
			}
			if( (flg & 2) != 0 )
			{
				br.skip(2);
			}

			const auto st = br.get_pos();
			const auto ef = br.get_size() - 8;
			if(st > ef)
				return false;
			f_inf.data_pos = st;
			f_inf.data_size = ef - st;

			br.set_pos(ef);
			br.readN(f_inf.crc32, 4);
			br.readC<4, endianness::LITTLE_ENDIAN>(f_inf.fsize);
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
		void close()
		{
			br.close();
		}

		std::string name() const
		{
			return f_inf.name;
		}

		bool GetData(std::vector<uint8_t> &data)
		{
			std::vector<uint8_t> tmp;
			getData(tmp);
			data.reserve(f_inf.fsize);
			if( !compr::deflate::Decode(tmp.data(), tmp.size(), data) )
				return false;
			return true;
		}
	};
}
