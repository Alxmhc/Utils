namespace lattice
{
	namespace sc
	{
		size_t num(size_t n1, size_t n2, size_t n3)
		{
			return n1*n2*n3;
		}

		std::vector<vt3> gen(size_t n1, size_t n2, size_t n3)
		{
			std::vector<vt3> r( num(n1,n2,n3) );
			vt3 d(0.5*(n1-1), 0.5*(n2-1), 0.5*(n3-1));

			size_t k = 0;
			for(size_t x=0; x<n1; x++)
			{
				for(size_t y=0; y<n2; y++)
				{
					for(size_t z=0; z<n3; z++)
					{
						r[k++] = vt3(x, y, z) - d;
					}
				}
			}
			return r;
		}
	}

	namespace bcc
	{
		size_t num(size_t n1, size_t n2, size_t n3)
		{
			return n1*n2*n3 + (n1-1)*(n2-1)*(n3-1);
		}

		std::vector<vt3> gen(size_t n1, size_t n2, size_t n3)
		{
			std::vector<vt3> r( num(n1,n2,n3) );
			vt3 d(0.5*(n1-1), 0.5*(n2-1), 0.5*(n3-1));

			size_t k = 0;
			for(size_t x=0; x<n1; x++)
			{
				for(size_t y=0; y<n2; y++)
				{
					for(size_t z=0; z<n3; z++)
					{
						r[k++] = vt3(x, y, z) - d;
						if(x && y && z){r[k++] = vt3(x-0.5, y-0.5, z-0.5) - d;}
					}
				}
			}
			return r;
		}
	}

	namespace fcc
	{
		size_t num(size_t n1, size_t n2, size_t n3)
		{
			return 2*(2*n1*n2*n3 - n1*n2 - n1*n3 - n2*n3) + n1 + n2 + n3;
		}

		std::vector<vt3> gen(size_t n1, size_t n2, size_t n3)
		{
			std::vector<vt3> r( num(n1,n2,n3) );
			vt3 d(0.5*(n1-1), 0.5*(n2-1), 0.5*(n3-1));

			size_t k = 0;
			for(size_t x=0; x<n1; x++)
			{
				for(size_t y=0; y<n2; y++)
				{
					for(size_t z=0; z<n3; z++)
					{
						r[k++] = vt3(x, y, z) - d;
						if(x && y){r[k++] = vt3(x-0.5, y-0.5, z) - d;}
						if(x && z){r[k++] = vt3(x-0.5, y, z-0.5) - d;}
						if(y && z){r[k++] = vt3(x, y-0.5, z-0.5) - d;}
					}
				}
			}
			return r;
		}
	}
}
