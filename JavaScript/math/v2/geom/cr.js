class p_cr{
	constructor(r){
		this.r = r;
	}
	sq(){
		return p_cr.sq(this.r);
	}
}
p_cr.sq = function(r){
	return Math.PI*r*r;
}
