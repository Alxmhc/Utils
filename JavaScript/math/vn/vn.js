class vn extends Float64Array{
	constructor(v){
		super(v);
	}
	clone(){return this.slice()}
	add(v){for(let i=0; i<this.length; ++i){this[i]+=v[i]} return this}
	sub(v){for(let i=0; i<this.length; ++i){this[i]-=v[i]} return this}
	mul(k){for(let i=0; i<this.length; ++i){this[i]*=k} return this}

	dot(v){return _arr.dot(this, v)}
	sum(){return _arr.sum(this)}
	avr(){return _arr.avr(this)}
}
