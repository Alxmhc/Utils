namespace fl_pr
{
	namespace F_7z
	{
		enum ID //from description
		{
			kEnd,
			kHeader,
			kArchiveProperties,
			kAdditionalStreamsInfo,
			kMainStreamsInfo,
			kFilesInfo,
			kPackInfo,
			kUnPackInfo,
			kSubStreamsInfo,
			kSize,
			kCRC,
			kFolder,
			kCodersUnPackSize,
			kNumUnPackStream,
			kEmptyStream,
			kEmptyFile,
			kAnti,
			kName,
			kCTime,
			kATime,
			kMTime,
			kWinAttributes,
			kComment,
			kEncodedHeader,
			kStartPos,
			kDummy
		};

		bool getNum(byteReader &s, uint_fast64_t &c)
		{
			uint8_t f;
			if(!s.get(f))
				return false;
			if((f & 0x80) == 0)
			{
				c = f;
				return true;
			}

			c = 0;
			uint_fast8_t m = 0x40, p = 0;
			uint8_t b;
			while(s.get(b))
			{
				c |= static_cast<uint_fast64_t>(b) << p;
				p += 8;
				if((f & m) == 0)
				{
					if(m > 1)
					{
						f &= m - 1;
						c |= static_cast<uint_fast64_t>(f) << p;
					}
					return true;
				}
				m >>= 1;
			}
			return false;
		}

		bool getHeader(byteReader &s, std::vector<uint8_t> &hdr)
		{
			uint8_t header[32];
			if(!s.readN(header, 32))
				return false;
			if(std::memcmp(header, "\x37\x7a\xbc\xaf\x27\x1c", 6) != 0)
				return false;
			if(header[6] != 0)
				return false;

			hash::CRC32 hs;
			uint8_t hash[hash::CRC32::hash_size];

			hs.Update(header + 12, 20);
			hs.Final(hash);
			if(std::memcmp(hash, header + 8, hash::CRC32::hash_size) != 0)
				return false;

			const auto NHsize = bconv<8, endianness::LITTLE_ENDIAN>::pack(header + 20);
			const auto NHoffset = bconv<8, endianness::LITTLE_ENDIAN>::pack(header + 12);
			s.set_pos(NHoffset, std::ios_base::cur);

			hdr.resize(NHsize);
			if(!s.readN(hdr.data(), NHsize))
				return false;
			hs.Update(hdr.data(), hdr.size());
			hs.Final(hash);
			if(std::memcmp(hash, header + 28, hash::CRC32::hash_size) != 0)
				return false;
			return true;
		}
	}
}
