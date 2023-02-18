#ifndef H_BIN_TREE
#define H_BIN_TREE

template<typename T>
class binTree
{
	binTree* left;
	binTree* right;
	T val;
	bool fin;

public:
	binTree() : left(nullptr), right(nullptr), fin(false){}

	binTree(binTree &&t) : left(t.left), right(t.right), val(t.val), fin(t.fin)
	{
		t.left = nullptr;
		t.right = nullptr;
	}

	~binTree()
	{
		delete left;
		delete right;
	}

	void add(uint_fast32_t n, uint_fast8_t nbit, T c)
	{
		uint_fast32_t f = 1 << (nbit - 1);
		binTree<T>* a = this;
		while(f != 0)
		{
			const auto c = n & f;
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
			f >>= 1;
		}
		a->val = c;
		a->fin = true;
	}

	bool find(bitReader &rd, T &c) const
	{
		const binTree<T>* a = this;
		for(;;)
		{
			uint_fast8_t n;
			if( !rd.get(n) )
				break;
			a = n == 0 ? a->left : a->right;
			if(a == nullptr)
				break;
			if(a->fin)
			{
				c = a->val;
				return true;
			}
		}
		return false;
	}
};

#endif
