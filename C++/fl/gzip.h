namespace fl_pr
{
	class F_gzip : public cont_1
	{
		std::string fname;
		uint_fast32_t fsize;
		uint8_t crc32[4];
		std::string comment;
	public:
		bool read(byteReader* r)
		{
			br = r;

			uint8_t flg;
			{
				uint8_t hdr[4];
				if( !br->readN(hdr, 4) )
					return false;
				if(std::memcmp(hdr, "\x1f\x8b\x08", 3) != 0)
					return false;
				flg = hdr[3];
			}
			if(flg > 31)
				return false;
			if( !br->skip(6) )
				return false;
			if( (flg & 4) != 0 )
			{
				uint_fast16_t sz;
				if (!br->readC<2, endianness::LITTLE_ENDIAN>(sz))
					return false;
				if (!br->skip(sz))
					return false;
			}
			if( (flg & 8) != 0 )
			{
				 if( !br->read_string(0, fname) )
					 return false;
			}
			if( (flg & 16) != 0 )
			{
				 if( !br->read_string(0, comment) )
					 return false;
			}
			if( (flg & 2) != 0 )
			{
				br->skip(2);
			}

			data_pos = br->get_pos();
			const auto ef = br->get_size() - 8;
			if(data_pos > ef)
				return false;
			data_size = ef - data_pos;

			br->set_pos(ef);
			br->readN(crc32, 4);
			br->readC<4, endianness::LITTLE_ENDIAN>(fsize);
			return true;
		}

		std::string name() const
		{
			return fname;
		}

		bool GetData(byteWriter &bw)
		{
			br->set_size(data_pos + data_size);
			br->set_pos(data_pos);
			if( !compr::deflate::Decode(*br, bw) )
				return false;
			return true;
		}
	};
}
