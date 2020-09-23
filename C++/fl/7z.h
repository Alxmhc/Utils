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

		bool getHeader(byteReader &s, std::vector<uint8_t> &hdr)
		{
			uint8_t header[32];
			if(s.read(header, 32) != 32)
				return false;
			if(std::memcmp(header, "\x37\x7a\xbc\xaf\x27\x1c", 6) != 0)
				return false;

			hash::CRC32 hs;
			uint8_t hash[hash::CRC32::hash_size];

			hs.Update(header + 12, 20);
			hs.Final(hash);
			if(std::memcmp(hash, header + 8, hash::CRC32::hash_size) != 0)
				return false;

			const auto NHoffset = bconv<8, endianness::LITTLE_ENDIAN>::pack(header + 12);
			const auto NHsize = bconv<8, endianness::LITTLE_ENDIAN>::pack(header + 20);
			s.skip(NHoffset);

			hdr.resize(NHsize);
			if(s.read(&hdr[0], NHsize) != NHsize)
				return false;
			hs.Update(hdr.data(), hdr.size());
			hs.Final(hash);
			if(std::memcmp(hash, header + 28, hash::CRC32::hash_size) != 0)
				return false;
			return true;
		}
	}
}
