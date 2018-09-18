function fct(n){
	return fctp(2, n);
}

function fctp(a, b){
	let r = 1;
	for(; a <= b; a++){
		r *= a;
	}
	return r;
}

function cf_bn(n, k){
	return k<n/2 ? fctp(n-k+1,n)/fct(k) : fctp(k+1,n)/fct(n-k);
}

function gcd(a, b){
	if(b > a){
		b %= a;
	}
	while(b){
		[a, b] = [b, a%b];
	}
	return a;
}

//(x^n)%p
function odp(x, n, p){
	if(x > p){
		x %= p;
	}
	let r = 1;
	while(x > 1){
		if(n & 1){
			r *= x;
			r %= p;
		}
		n >>= 1;
		if(n == 0)
			break
		x *= x;
		x %= p;
	}
	return x ? r : 0;
}
