namespace fl_pr
{
	class F_pcapng
	{
		byteReader* br;

		struct inf
		{
			uint_fast32_t type;
			std::vector<uint8_t> data;
		};

		bool read1(inf &r)
		{
			if( !br->readC<4, endianness::LITTLE_ENDIAN>(r.type) )
				return false;
			uint_fast32_t size;
			if( !br->readC<4, endianness::LITTLE_ENDIAN>(size)
			|| size < 12 )
				return false;
			if( !br->readN(r.data, size - 12) )
				return false;
			if( !br->skip(4) )
				return false;
			return true;
		}
	public:
		bool read(byteReader* r)
		{
			br = r;
			return true;
		}

		bool nxt(std::vector<uint8_t> &data)
		{
			inf r;
			if( !read1(r) )
				return false;
			if(r.type == 6)
			{
				const auto size = bconv<4, endianness::LITTLE_ENDIAN>::pack(r.data.data() + 12);
				data.assign(r.data.cbegin() + 20, r.data.cbegin() + 20 + size);
			}
			else
			{
				data.clear();
			}
			return true;
		}
	};
}
