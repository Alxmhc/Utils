//f(x)=0; a<=x<=b
function solve_eq(a,b,f,d){
	const x = f(a);
	if(x>-d && x<d)
		return a;
	const y = f(b);
	if(y>-d && y<d)
		return b;
	if(x<0 && y>0)
		return _eq_half(a,b,f,d);
	if(x>0 && y<0)
		return _eq_half(b,a,f,d);
	return null;
}

function _eq_half(a,b,f,d){
	for(;;){
		x = 0.5*(a+b);
		y = f(x);
		if(y < 0){
			if(y > -d)
				return x;
			a = x;
		}
		else{
			if(y < d)
				return x;
			b = x;
		}
	}
}

//a.x*r1+a.y*r2+a.z=0; b.x*r1+b.y*r2+b.z=0
function solv2l(a,b){
	let q = vt3.pr_v3(a,b);
	return new vt2(q.x/q.z, q.y/q.z);
}

//y=r1+r2*x approximation
function linapr(x,y){
	let n=x.length, a=new vt3(n,0,0), b=new vt3(0,0,0);
	for(;n;){
		n--;
		a.y += x[n];
		b.y += x[n]*x[n];
		a.z -= y[n];
		b.z -= x[n]*y[n];
	}
	b.x = a.y;
	return solv2l(a,b);
}

//y=r1+r2/x approximation
function gipapr(x,y){
	let n=x.length, a=new vt3(n,0,0), b=new vt3(0,0,0);
	for(;n;){
		n--;
		q = 1.0/x[n];
		a.y += q;
		b.y += q*q;
		a.z -= y[n];
		b.z -= y[n]*q;
	}
	b.x = a.y;
	return solv2l(a,b);
}
