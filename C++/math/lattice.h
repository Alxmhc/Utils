#ifndef H_LATTICE
#define H_LATTICE

namespace lattice
{
	namespace sc
	{
		size_t num(uint_fast32_t n1, uint_fast32_t n2, uint_fast32_t n3)
		{
			return n1*n2*n3;
		}

		std::vector<vt3> gen(uint_fast32_t n1, uint_fast32_t n2, uint_fast32_t n3)
		{
			std::vector<vt3> r( num(n1,n2,n3) );
			vt3 d(0.5*(n1-1), 0.5*(n2-1), 0.5*(n3-1));

			size_t k = 0;
			for(uint_fast32_t x=0; x<n1; x++)
			{
				for(uint_fast32_t y=0; y<n2; y++)
				{
					for(uint_fast32_t z=0; z<n3; z++)
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
		size_t num(uint_fast32_t n1, uint_fast32_t n2, uint_fast32_t n3)
		{
			return n1*n2*n3 + (n1-1)*(n2-1)*(n3-1);
		}

		std::vector<vt3> gen(uint_fast32_t n1, uint_fast32_t n2, uint_fast32_t n3)
		{
			std::vector<vt3> r( num(n1,n2,n3) );
			vt3 d(0.5*(n1-1), 0.5*(n2-1), 0.5*(n3-1));

			size_t k = 0;
			for(uint_fast32_t x=0; x<n1; x++)
			{
				for(uint_fast32_t y=0; y<n2; y++)
				{
					for(uint_fast32_t z=0; z<n3; z++)
					{
						r[k++] = vt3(x, y, z) - d;
						if(x && y && z)
						{
							r[k++] = vt3(x-0.5, y-0.5, z-0.5) - d;
						}
					}
				}
			}
			return r;
		}
	}

	namespace fcc
	{
		size_t num(uint_fast32_t n1, uint_fast32_t n2, uint_fast32_t n3)
		{
			return 2*(2*n1*n2*n3 - n1*n2 - n1*n3 - n2*n3) + n1 + n2 + n3;
		}

		std::vector<vt3> gen(uint_fast32_t n1, uint_fast32_t n2, uint_fast32_t n3)
		{
			std::vector<vt3> r( num(n1,n2,n3) );
			vt3 d(0.5*(n1-1), 0.5*(n2-1), 0.5*(n3-1));

			size_t k = 0;
			for(uint_fast32_t x=0; x<n1; x++)
			{
				for(uint_fast32_t y=0; y<n2; y++)
				{
					for(uint_fast32_t z=0; z<n3; z++)
					{
						r[k++] = vt3(x, y, z) - d;
						if(x && y)
						{
							r[k++] = vt3(x-0.5, y-0.5, z) - d;
						}
						if(x && z)
						{
							r[k++] = vt3(x-0.5, y, z-0.5) - d;
						}
						if(y && z)
						{
							r[k++] = vt3(x, y-0.5, z-0.5) - d;
						}
					}
				}
			}
			return r;
		}
	}
}

#endif
