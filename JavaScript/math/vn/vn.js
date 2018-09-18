class vn extends Float64Array{
	constructor(v){
		super(v);
	}
	clone(){return this.slice()}
	add(v){for(let i=0; i<this.length; i++){this[i]+=v[i]} return this}
	sub(v){for(let i=0; i<this.length; i++){this[i]-=v[i]} return this}
	mul(k){for(let i=0; i<this.length; i++){this[i]*=k} return this}

	dot(v){let s=0; for(let i=0; i<this.length; i++){s+=this[i]*v[i]} return s}
	sum(){return vn.sum(this)}
	avr(){return vn.avr(this)}
}
vn.sum = function(v){return v.reduce((s,e)=>s+e,0)}
vn.avr = function(v){const n=v.length; return n!==0 ? vn.sum(v)/n : 0}
