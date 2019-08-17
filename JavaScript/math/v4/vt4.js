class vt4 extends Float64Array{
	constructor(a){
		super(4);
		if(a){
			this.set(a);
		}
	}
	clone(){return this.slice()}
	copy(v){this[0]=v[0]; this[1]=v[1]; this[2]=v[2]; this[3]=v[3]; return this}
	add(v){this[0]+=v[0]; this[1]+=v[1]; this[2]+=v[2]; this[3]+=v[3]; return this}
	sub(v){this[0]-=v[0]; this[1]-=v[1]; this[2]-=v[2]; this[3]-=v[3]; return this}
	mul(k){this[0]*=k; this[1]*=k; this[2]*=k; this[3]*=k; return this}
	dot(v){return this[0]*v[0]+this[1]*v[1]+this[2]*v[2]+this[3]*v[3]}

	dot_m(m){
		const a = this[0], b = this[1], c = this[2], d = this[3];
		this[0] =  a*m[0] +  b*m[1] +  c*m[2] +  d*m[3];
		this[1] =  a*m[4] +  b*m[5] +  c*m[6] +  d*m[7];
		this[2] =  a*m[8] +  b*m[9] + c*m[10] + d*m[11];
		this[3] = a*m[12] + b*m[13] + c*m[14] + d*m[15];
		return this;
	}
}
