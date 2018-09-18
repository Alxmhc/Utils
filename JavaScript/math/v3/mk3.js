class mk3 extends Float64Array{
	constructor(m){
		super(9);
		if(m){
			this.set(m);
		}
		this.sx=3;
		this.sy=3;
	}
	clone(){return this.slice()}
	add(m){
		this[0] += m[0]; this[1] += m[1]; this[2] += m[2];
		this[3] += m[3]; this[4] += m[4]; this[5] += m[5];
		this[6] += m[6]; this[7] += m[7]; this[8] += m[8];
		return this;
	}
	sub(m){
		this[0] -= m[0]; this[1] -= m[1]; this[2] -= m[2];
		this[3] -= m[3]; this[4] -= m[4]; this[5] -= m[5];
		this[6] -= m[6]; this[7] -= m[7]; this[8] -= m[8];
		return this;
	}
	mul(k){
		this[0] *= k; this[1] *= k; this[2] *= k;
		this[3] *= k; this[4] *= k; this[5] *= k;
		this[6] *= k; this[7] *= k; this[8] *= k;
		return this;
	}

	getx(k){return new vt3(this[3*k], this[3*k+1], this[3*k+2])}
	gety(k){return new vt3(this[k], this[k+3], this[k+6])}
	set_v(v1, v2, v3){
		this.set([v1[0], v1[1], v1[2],
		          v2[0], v2[1], v2[2],
		          v3[0], v3[1], v3[2]]);
		return this;
	}
	det(){
		const a0 = this[4]*this[8] - this[5]*this[7],
		      a1 = this[5]*this[6] - this[3]*this[8],
		      a2 = this[3]*this[7] - this[4]*this[6];
		return this[0]*a0 + this[1]*a1 + this[2]*a2;
	}
	dot(m){
		let a = this[0], b = this[1], c = this[2];
		this[0] = a*m[0] + b*m[3] + c*m[6];
		this[1] = a*m[1] + b*m[4] + c*m[7];
		this[2] = a*m[2] + b*m[5] + c*m[8];
		a = this[3]; b = this[4]; c = this[5];
		this[3] = a*m[0] + b*m[3] + c*m[6];
		this[4] = a*m[1] + b*m[4] + c*m[7];
		this[5] = a*m[2] + b*m[5] + c*m[8];
		a = this[6]; b = this[7]; c = this[8];
		this[6] = a*m[0] + b*m[3] + c*m[6];
		this[7] = a*m[1] + b*m[4] + c*m[7];
		this[8] = a*m[2] + b*m[5] + c*m[8];
		return this;
	}
	inverse(){
		const a0 = this[4]*this[8] - this[5]*this[7],
		      a1 = this[5]*this[6] - this[3]*this[8],
		      a2 = this[3]*this[7] - this[4]*this[6];
		const d = this[0]*a0 + this[1]*a1 + this[2]*a2;
		if(d === 0)
			return null;
		this.set([a0, this[2]*this[7]-this[1]*this[8], this[1]*this[5]-this[2]*this[4],
		          a1, this[0]*this[8]-this[2]*this[6], this[2]*this[3]-this[0]*this[5],
		          a2, this[1]*this[6]-this[0]*this[7], this[0]*this[4]-this[1]*this[3]]);
		return this.mul(1.0/d);
	}
}
mk3.from_v = function(v1, v2, v3){
	let r = new mk3();
	return r.set_v(v1, v2, v3);
}
