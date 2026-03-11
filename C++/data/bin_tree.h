#ifndef H_BIN_TREE
#define H_BIN_TREE

#include "bit_reader.h"

template<typename T>
class binTree
{
	binTree* left;
	binTree* right;
	bool fin;
	T val;

public:
	binTree() : left(nullptr), right(nullptr), fin(false){}

	binTree(binTree &&t) : left(t.left), right(t.right), fin(t.fin), val(t.val)
	{
		t.left = nullptr;
		t.right = nullptr;
	}

	binTree& operator=(binTree &&t)
	{
		if(this != &t)
		{
			delete left;
			left = t.left;
			t.left = nullptr;

			delete right;
			right = t.right;
			t.right = nullptr;

			fin = t.fin;
			t.fin = false;

			val = t.val;
		}
		return *this;
	}

	~binTree()
	{
		delete left;
		delete right;
	}

	bool add(uint_fast64_t n, uint_fast8_t nbit, const T &v)
	{
		binTree<T>* a = this;
		while(nbit--)
		{
			if (a->fin)
				return false;
			const auto c = (n >> nbit) & 1;
			if(c == 0)
			{
				if(a->left == nullptr)
				{
					a->left = new binTree();
				}
				a = a->left;
			}
			else
			{
				if(a->right == nullptr)
				{
					a->right = new binTree();
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

	bool decode(bitReader &rd, T &res) const
	{
		const binTree<T>* tr = this;
		for(;;)
		{
			if (tr->fin)
			{
				res = tr->val;
				return true;
			}
			uint_fast8_t b;
			if( !rd.get(b) )
				return false;
			tr = b == 0 ? tr->left : tr->right;
			if(tr == nullptr)
				return false;
		}
	}
};

#endif
