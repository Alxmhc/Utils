class vt2 extends Float64Array{
	constructor(a){
		super(2);
		if(a){
			this.set(a);
		}
	}
	clone(){return this.slice()}
	copy(v){this[0]=v[0]; this[1]=v[1]; return this}
	add(v){this[0]+=v[0]; this[1]+=v[1]; return this}
	sub(v){this[0]-=v[0]; this[1]-=v[1]; return this}
	mul(k){this[0]*=k; this[1]*=k; return this}
	dot(v){return this[0]*v[0]+this[1]*v[1]}

	get x(){return this[0]}
	set x(v){this[0] = v}
	get y(){return this[1]}
	set y(v){this[1] = v}
	dot_m(m){
		const a = this[0], b = this[1];
		this[0] = a*m[0] + b*m[1];
		this[1] = a*m[2] + b*m[3];
		return this;
	}
}
