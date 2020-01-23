template<typename T>
class binTree
{
	binTree *left, *right;
	T val;
	bool fin;

	binTree* get_left()
	{
		if(left == nullptr)
		{
			left = new binTree();
		}
		return left;
	}
	binTree* get_right()
	{
		if(right == nullptr)
		{
			right = new binTree();
		}
		return right;
	}

public:
	binTree() : left(nullptr), right(nullptr), fin(false){}

	binTree(binTree &&t) : left(t.left), right(t.right), fin(t.fin), val(t.val)
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
		binTree<T> *a = this;
		while(f != 0)
		{
			const auto c = n & f;
			f >>= 1;
			a = c == 0 ? a->get_left() : a->get_right();
		}
		a->val = c;
		a->fin = true;
	}

	bool find(bitReader &rd, T &c) const
	{
		const binTree<T> *a = this;
		for(;;)
		{
			const auto n = rd.read1();
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