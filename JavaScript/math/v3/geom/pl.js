//n*r-p=0; |n|=1; p>=0
class pl{
	#dst(k){
		return this.n.dot(k) - this.p
	}
	constructor(n,p){
		this.n = n
		this.p = p
	}

	dst(k){
		return Math.abs(this.#dst(k))
	}
}
pl.from_3p = function(a,b,c){
	let n = vt3.pr(b.clone().sub(a), c.clone().sub(a))
	_v.nrm(n)
	let p = a.dot(n)
	if(p < 0){
		p = -p
		n.mul(-1)
	}
	return new pl(n,p)
}
