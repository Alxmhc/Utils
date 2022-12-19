namespace fl_pr
{
	class F_pcapng
	{
		byteReader* br;
	public:
		bool read(byteReader* r)
		{
			br = r;
			return true;
		}

		bool nxt(std::vector<uint8_t> &data)
		{
			uint_fast32_t type;
			if( !br->readC<4, endianness::LITTLE_ENDIAN>(type) )
				return false;
			uint_fast32_t size;
			if( !br->readC<4, endianness::LITTLE_ENDIAN>(size)
			|| size < 12 )
				return false;

			std::vector<uint8_t> tmp;
			if( !br->readN(tmp, size - 12) )
				return false;
			if( !br->skip(4) )
				return false;

			if(type == 6)
			{
				const auto size = bconv<4, endianness::LITTLE_ENDIAN>::pack(tmp.data() + 12);
				data.assign(tmp.cbegin() + 20, tmp.cbegin() + 20 + size);
			}
			else
			{
				data.clear();
			}
			return true;
		}
	};
}
