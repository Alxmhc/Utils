class mk4 extends Float64Array{
	constructor(m){
		super(16);
		if(m){
			this.set(m);
		}
		this.sx=4;
		this.sy=4;
	}
	clone(){return this.slice()}
	add(m){
		this[0] += m[0]; this[1] += m[1]; this[2] += m[2]; this[3] += m[3];
		this[4] += m[4]; this[5] += m[5]; this[6] += m[6]; this[7] += m[7];
		this[8] += m[8]; this[9] += m[9]; this[10] += m[10]; this[11] += m[11];
		this[12] += m[12]; this[13] += m[13]; this[14] += m[14]; this[15] += m[15];
		return this;
	}
	sub(m){
		this[0] -= m[0]; this[1] -= m[1]; this[2] -= m[2]; this[3] -= m[3];
		this[4] -= m[4]; this[5] -= m[5]; this[6] -= m[6]; this[7] -= m[7];
		this[8] -= m[8]; this[9] -= m[9]; this[10] -= m[10]; this[11] -= m[11];
		this[12] -= m[12]; this[13] -= m[13]; this[14] -= m[14]; this[15] -= m[15];
		return this;
	}
	mul(k){
		this[0] *= k; this[1] *= k; this[2] *= k; this[3] *= k;
		this[4] *= k; this[5] *= k; this[6] *= k; this[7] *= k;
		this[8] *= k; this[9] *= k; this[10] *= k; this[11] *= k;
		this[12] *= k; this[13] *= k; this[14] *= k; this[15] *= k;
		return this;
	}

	getx(k){return new vt4(this[4*k], this[4*k+1], this[4*k+2], this[4*k+3])}
	gety(k){return new vt4(this[k], this[k+4], this[k+8], this[k+12])}
	set_v(v1, v2, v3, v4){
		this.set([v1[0], v1[1], v1[2], v1[3],
		          v2[0], v2[1], v2[2], v2[3],
		          v3[0], v3[1], v3[2], v3[3],
		          v4[0], v4[1], v4[2], v4[3]]);
		return this;
	}
	det(){
		const q0 = this[8]*this[13] - this[9]*this[12],
		      q1 = this[8]*this[14] - this[10]*this[12],
		      q2 = this[8]*this[15] - this[11]*this[12],
		      q3 = this[9]*this[14] - this[10]*this[13],
		      q4 = this[9]*this[15] - this[11]*this[13],
		      q5 = this[10]*this[15] - this[11]*this[14];
		const a0 =  this[5]*q5 - this[6]*q4 + this[7]*q3,
		      a1 = -this[4]*q5 + this[6]*q2 - this[7]*q1,
		      a2 =  this[4]*q4 - this[5]*q2 + this[7]*q0,
		      a3 = -this[4]*q3 + this[5]*q1 - this[6]*q0;
		return this[0]*a0 + this[1]*a1 + this[2]*a2 + this[3]*a3;
	}
	dot(m){
		if(this === m){
			m = m.clone();
		}
		let a = this[0], b = this[1], c = this[2], d = this[3];
		this[0] = a*m[0] + b*m[4] + c*m[8] + d*m[12];
		this[1] = a*m[1] + b*m[5] + c*m[9] + d*m[13];
		this[2] = a*m[2] + b*m[6] + c*m[10] + d*m[14];
		this[3] = a*m[3] + b*m[7] + c*m[11] + d*m[15];
		a = this[4]; b = this[5]; c = this[6]; d = this[7];
		this[4] = a*m[0] + b*m[4] + c*m[8] + d*m[12];
		this[5] = a*m[1] + b*m[5] + c*m[9] + d*m[13];
		this[6] = a*m[2] + b*m[6] + c*m[10] + d*m[14];
		this[7] = a*m[3] + b*m[7] + c*m[11] + d*m[15];
		a = this[8]; b = this[9]; c = this[10]; d = this[11];
		this[8] = a*m[0] + b*m[4] + c*m[8] + d*m[12];
		this[9] = a*m[1] + b*m[5] + c*m[9] + d*m[13];
		this[10] = a*m[2] + b*m[6] + c*m[10] + d*m[14];
		this[11] = a*m[3] + b*m[7] + c*m[11] + d*m[15];
		a = this[12]; b = this[13]; c = this[14]; d = this[15];
		this[12] = a*m[0] + b*m[4] + c*m[8] + d*m[12];
		this[13] = a*m[1] + b*m[5] + c*m[9] + d*m[13];
		this[14] = a*m[2] + b*m[6] + c*m[10] + d*m[14];
		this[15] = a*m[3] + b*m[7] + c*m[11] + d*m[15];
		return this;
	}
	inverse(){
		const q0 = this[8]*this[13] - this[9]*this[12],
		      q1 = this[8]*this[14] - this[10]*this[12],
		      q2 = this[8]*this[15] - this[11]*this[12],
		      q3 = this[9]*this[14] - this[10]*this[13],
		      q4 = this[9]*this[15] - this[11]*this[13],
		      q5 = this[10]*this[15] - this[11]*this[14];
		const a0 =  this[5]*q5 - this[6]*q4 + this[7]*q3,
		      a1 = -this[4]*q5 + this[6]*q2 - this[7]*q1,
		      a2 =  this[4]*q4 - this[5]*q2 + this[7]*q0,
		      a3 = -this[4]*q3 + this[5]*q1 - this[6]*q0;
		const d = this[0]*a0 + this[1]*a1 + this[2]*a2 + this[3]*a3;
		if(d === 0)
			return null;
		const t0 = this[6]*this[15] - this[7]*this[14],
		      t1 = this[5]*this[15] - this[7]*this[13],
		      t2 = this[5]*this[14] - this[6]*this[13],
		      t3 = this[6]*this[11] - this[7]*this[10],
		      t4 = this[5]*this[11] - this[7]*this[9],
		      t5 = this[5]*this[10] - this[6]*this[9],
		      t6 = this[4]*this[15] - this[7]*this[12],
		      t7 = this[4]*this[14] - this[6]*this[12],
		      t8 = this[4]*this[11] - this[7]*this[8],
		      t9 = this[4]*this[10] - this[6]*this[8],
		      t10 = this[4]*this[13] - this[5]*this[12],
		      t11 = this[4]*this[9] - this[5]*this[8];
		this.set([a0, -this[1]*q5+this[2]*q4-this[3]*q3, this[1]*t0-this[2]*t1+this[3]*t2, -this[1]*t3+this[2]*t4-this[3]*t5,
		          a1, this[0]*q5-this[2]*q2+this[3]*q1, -this[0]*t0+this[2]*t6-this[3]*t7, this[0]*t3-this[2]*t8+this[3]*t9,
		          a2, -this[0]*q4+this[1]*q2-this[3]*q0, this[0]*t1-this[1]*t6+this[3]*t10, -this[0]*t4+this[1]*t8-this[3]*t11,
		          a3, this[0]*q3-this[1]*q1+this[2]*q0, -this[0]*t2+this[1]*t7-this[2]*t10, this[0]*t5-this[1]*t9+this[2]*t11]);
		return this.mul(1.0/d);
	}
}
mk4.from_v = function(v1, v2, v3, v4){
	let r = new mk4();
	return r.set_v(v1, v2, v3, v4);
}
