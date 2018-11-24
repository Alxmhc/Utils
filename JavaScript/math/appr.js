//y=a+b*x
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
	return eq1_2(a,b);
}

//y=a+b/x
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
	return eq1_2(a,b);
}
