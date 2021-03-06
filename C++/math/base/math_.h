const long double pi = 3.1415926535897932384626;

template<typename T>
T operator*(const T &a, long double k){T r(a); r *= k; return r;}

template<typename T>
const T& operator/=(T &a, long double k){return a*=(1.0/k);}

template<typename T>
T operator/(const T &a, long double k){return a*(1.0/k);}
