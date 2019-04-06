class mk2 extends Float64Array{
	constructor(m){
		super(4);
		if(m){
			this.set(m);
		}
		this.sx=2;
		this.sy=2;
	}
	clone(){return this.slice()}
	copy(m){
		this.set(m);
		return this;
	}
	add(m){
		this[0] += m[0]; this[1] += m[1];
		this[2] += m[2]; this[3] += m[3];
		return this;
	}
	sub(m){
		this[0] -= m[0]; this[1] -= m[1];
		this[2] -= m[2]; this[3] -= m[3];
		return this;
	}
	mul(k){
		this[0] *= k; this[1] *= k;
		this[2] *= k; this[3] *= k;
		return this;
	}

	getx(k){return new vt2(this[2*k], this[2*k+1])}
	gety(k){return new vt2(this[k], this[k+2])}
	set_v(v1, v2){
		this.set([v1[0], v1[1],
		          v2[0], v2[1]]);
		return this;
	}
	det(){
		return this[0]*this[3]-this[1]*this[2];
	}
	dot(m){
		const a0 = this[0]*m[0] + this[1]*m[2],
		      a1 = this[0]*m[1] + this[1]*m[3],
		      a2 = this[2]*m[0] + this[3]*m[2],
		      a3 = this[2]*m[1] + this[3]*m[3];
		this.set([a0,a1,a2,a3]);
		return this;
	}
	transpose(){
		[this[1],this[2]] = [this[2],this[1]];
		return this;
	}
	inverse(){
		const d = this[0]*this[3]-this[1]*this[2];
		if(d === 0)
			return null;
		this.set([this[3], -this[1],
		          -this[2], this[0]]);
		return this.mul(1.0/d);
	}
}
mk2.from_v = function(v1, v2){
	let r = new mk2();
	return r.set_v(v1, v2);
}
