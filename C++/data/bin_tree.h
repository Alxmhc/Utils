#ifndef H_BIN_TREE
#define H_BIN_TREE

#include <map>

#include "bit_reader.h"
#include "bit_writer.h"

template<typename T>
struct binTree
{
	class Dec
	{
		Dec* left;
		Dec* right;
		bool fin;
		T val;
	public:
		Dec() : left(nullptr), right(nullptr), fin(false) {}

		Dec(Dec&& t) noexcept : left(t.left), right(t.right), fin(t.fin), val(t.val)
		{
			t.left = nullptr;
			t.right = nullptr;
			t.fin = false;
		}

		Dec& operator=(Dec &&t) noexcept
		{
			if (this != &t)
			{
				delete left;
				left = t.left;
				t.left = nullptr;

				delete right;
				right = t.right;
				t.right = nullptr;

				fin = t.fin;
				if (fin)
				{
					val = std::move(t.val);
					t.fin = false;
				}
			}
			return *this;
		}

		~Dec()
		{
			delete left;
			delete right;
		}

		bool add(uint_fast64_t n, uint_fast8_t nbit, const T &v)
		{
			Dec* a = this;
			while (nbit--)
			{
				if (a->fin)
					return false;
				const uint_fast8_t c = (n >> nbit) & 1;
				if (c == 0)
				{
					if (a->left == nullptr)
					{
						a->left = new Dec();
					}
					a = a->left;
				}
				else
				{
					if (a->right == nullptr)
					{
						a->right = new Dec();
					}
					a = a->right;
				}
			}
			if (a->fin || a->left != nullptr || a->right != nullptr)
				return false;
			a->val = v;
			a->fin = true;
			return true;
		}

		bool read(bitReader &rd, T &res) const
		{
			const Dec* tr = this;
			for (;;)
			{
				if (tr->fin)
				{
					res = tr->val;
					return true;
				}
				uint_fast8_t b;
				if (!rd.get(b))
					return false;
				tr = b == 0 ? tr->left : tr->right;
				if (tr == nullptr)
					return false;
			}
		}
	};

	class Enc
	{
		std::map<T, std::pair<uint_fast64_t, uint_fast8_t>> mp;
	public:
		bool add(uint_fast64_t n, uint_fast8_t nbit, const T &v)
		{
			if (mp.find(v) != mp.end())
				return false;
			mp[v] = std::pair<uint_fast64_t, uint_fast8_t>(n, nbit);
			return true;
		}

		bool write(const T &v, bitWriterL &bw) const
		{
			const auto e = mp.find(v);
			if (e == mp.cend())
				return false;
			bw.writeLE(e->second.first, e->second.second);
			return true;
		}
		bool write(const T &v, bitWriterR &bw) const
		{
			const auto e = mp.find(v);
			if (e == mp.cend())
				return false;
			bw.writeBE(e->second.first, e->second.second);
			return true;
		}

		void make_decoder(Dec &d) const
		{
			for (const auto& e : mp)
			{
				d.add(e.second.first, e.second.second, e.first);
			}
		}
	};
};

#endif
