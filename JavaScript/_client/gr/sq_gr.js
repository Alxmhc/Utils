class f_sqr extends fld_2d{
	#resz(){
		this.csz = [this.size[0]/this.cn[0], this.size[1]/this.cn[1]];
	}

	constructor(id,n){
		super(id);
		this.cn = n;
		this.#resz();
	}
	resize(sz){
		super.resize(sz);
		this.#resz();
	}

	get csx(){
		return this.cn[0];
	}
	get csy(){
		return this.cn[1];
	}

	//cell from coordinates
	crd_e(c){
		const icr = this.crd(c);
		return this.crd_c(icr);
	}
	//cell from internal coordinates
	crd_c(c){
		return [Math.floor(c[0]/this.csz[0]), Math.floor(c[1]/this.csz[1])];
	}

	del(cr){
		this.c.clearRect(this.csz[0]*cr[0], this.csz[1]*cr[1], this.csz[0], this.csz[1]);
	}
	pct(cr,p){
		this.draw(p, [this.csz[0]*cr[0], this.csz[1]*cr[1]], [this.csz[0], this.csz[1]]);
	}
	fll(cr){
		this.c.fillRect(this.csz[0]*cr[0], this.csz[1]*cr[1], this.csz[0], this.csz[1]);
	}
}
