class mk3 extends Float64Array{
	constructor(m){
		super(9);
		if(m){
			this.set(m);
		}
		this.sx = 3;
		this.sy = 3;
	}
	clone(){
		return this.slice();
	}
	copy(m){
		this.set(m);
		return this;
	}
	setI(){
		return this.copy([1,0,0,0,1,0,0,0,1]);
	}
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

	getx(k){
		return new vt3([this[3*k], this[3*k+1], this[3*k+2]]);
	}
	gety(k){
		return new vt3([this[k], this[k+3], this[k+6]]);
	}
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
		const a0 = this[0]*m[0] + this[1]*m[3] + this[2]*m[6],
		      a1 = this[0]*m[1] + this[1]*m[4] + this[2]*m[7],
		      a2 = this[0]*m[2] + this[1]*m[5] + this[2]*m[8],
		      a3 = this[3]*m[0] + this[4]*m[3] + this[5]*m[6],
		      a4 = this[3]*m[1] + this[4]*m[4] + this[5]*m[7],
		      a5 = this[3]*m[2] + this[4]*m[5] + this[5]*m[8],
		      a6 = this[6]*m[0] + this[7]*m[3] + this[8]*m[6],
		      a7 = this[6]*m[1] + this[7]*m[4] + this[8]*m[7],
		      a8 = this[6]*m[2] + this[7]*m[5] + this[8]*m[8];
		return this.copy([a0,a1,a2,a3,a4,a5,a6,a7,a8]);
	}
	transpose(){
		[this[1],this[3]] = [this[3],this[1]];
		[this[2],this[6]] = [this[6],this[2]];
		[this[5],this[7]] = [this[7],this[5]];
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
mk3.I = function(){
	let r = new mk3();
	return r.setI();
}
mk3.from_v = function(v1, v2, v3){
	let r = new mk3();
	return r.set_v(v1, v2, v3);
}
