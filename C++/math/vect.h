template<typename T>
T cntr(std::vector<T> &v)
{
	T a = avr(v);
	v -= a;
	return a;
}

//find point clusters with distance <= d; numbering from 1
template <typename T>
std::vector<unsigned int> cls(const std::vector<T> &v, long double d)
{
	auto n = v.size();
	std::vector<unsigned int> r(n);
	unsigned int k = 0;
	std::deque<unsigned int> q;
	for(unsigned int i = 0; i < n; i++)
	{
		if (r[i] != 0)
			continue;
		k++;
		r[i]=k;
		q.push_back(i);
		while(!q.empty())
		{
			const T p = v[q.front()];
			q.pop_front();
			for(unsigned int j = i+1; j < n; j++)
			{
				if( (r[j] == 0) && (abs(v[j]-p) <= d) )
				{
					r[j] = k;
					q.push_back(j);
				}
			}
		}
	}
	return r;
}
