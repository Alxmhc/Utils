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
}
