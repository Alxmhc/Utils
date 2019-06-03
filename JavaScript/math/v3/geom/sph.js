class p_sph{
	constructor(r){
		this.r = r;
	}
	vl(){
		return p_sph.vl(this.r);
	}
}
p_sph.vl = function(r){
	return 4.0*(Math.PI/3.0)*r*r*r;
}

class sph extends p_sph{
	constructor(c,r){
		super(r);
		this.c = c;
	}
}
