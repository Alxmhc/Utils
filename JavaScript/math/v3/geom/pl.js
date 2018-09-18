class pl{
	_dst(k){return this.n.dot(k)-this.p}
	constructor(n,p){this.n=n;this.p=p}

	dst(k){return Math.abs(this._dst(k))}
}
pl.from_3p = function(a,b,c){
	let n = vt3.pr(b.clone().sub(a), c.clone().sub(a));
	_v.nrm_s(n);
	let p = a.dot(n);
	if(p < 0){p = -p; n.mul(-1)}
	return new pl(n,p)
}
