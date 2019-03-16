class vt3 extends Float64Array{
	constructor(x, y, z){super([x,y,z])}

	clone(){return this.slice()}
	copy(v){this[0]=v[0]; this[1]=v[1]; this[2]=v[2]; return this}
	add(v){this[0]+=v[0]; this[1]+=v[1]; this[2]+=v[2]; return this}
	sub(v){this[0]-=v[0]; this[1]-=v[1]; this[2]-=v[2]; return this}
	mul(k){this[0]*=k; this[1]*=k; this[2]*=k; return this}
	dot(v){return this[0]*v[0]+this[1]*v[1]+this[2]*v[2]}

	get x(){return this[0]}
	set x(v){this[0] = v}
	get y(){return this[1]}
	set y(v){this[1] = v}
	get z(){return this[2]}
	set z(v){this[2] = v}
	dot_m(m){
		const a = this[0], b = this[1], c = this[2];
		this[0] = a*m[0] + b*m[1] + c*m[2];
		this[1] = a*m[3] + b*m[4] + c*m[5];
		this[2] = a*m[6] + b*m[7] + c*m[8];
		return this;
	}
}
vt3.pr = function(a,b){return new vt3(a[1]*b[2]-a[2]*b[1], a[2]*b[0]-a[0]*b[2], a[0]*b[1]-a[1]*b[0])}
