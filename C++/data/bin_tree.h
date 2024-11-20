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

	void add(uint_fast64_t n, uint_fast8_t nbit, T c)
	{
		binTree<T>* a = this;
		while(nbit != 0)
		{
			nbit--;
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
		a->val = c;
		a->fin = true;
	}

	const binTree* nxt(uint_fast8_t b) const
	{
		return b == 0 ? left : right;
	}

	bool get_val(T &res) const
	{
		if(!fin)
			return false;
		res = val;
		return true;
	}
};

template<typename T>
bool btree_decode(const binTree<T>* tr, bitReader &rd, T &res)
{
	for(;;)
	{
		if(tr == nullptr)
			return false;
		if(tr->get_val(res))
			return true;
		uint8_t b;
		if( !rd.get(b) )
			return false;
		tr = tr->nxt(b);
	}
}

#endif
