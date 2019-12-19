namespace hash
{
	template<std::size_t hsz>
	class hash
	{
		virtual void Final(std::array<uint8_t, hsz>&) = 0;
	protected:
		uint64_t size;
	public:
		static const uint_fast16_t hash_size = hsz;

		std::vector<uint8_t> Result()
		{
			std::array<uint8_t, hsz> tmp;
			Final(tmp);
			return std::vector<uint8_t>(tmp.cbegin(), tmp.cend());
		}
	};
}
