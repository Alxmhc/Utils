const long double pi = 3.1415926535897932384626;

template<typename T>
const T& operator/=(T &a, long double k){return a*=(1.0/k);}

template<typename T>
T operator/(const T &a, long double k){return a*(1.0/k);}

template<typename T>
long double abs(const T &a){return sqrt(a*a);}

template<typename T>
void nrm(T &v){v/=abs(v);}
