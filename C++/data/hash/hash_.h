namespace hash
{
	template<std::size_t hsz>
	class hash
	{
	protected:
		uint64_t size;
	public:
		static const uint_fast16_t hash_size = hsz;

		virtual void Update(byteReader&) = 0;
		virtual void Final(std::array<uint8_t, hsz>&) = 0;
	};
}
