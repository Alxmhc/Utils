class f_sqr extends fld_2d{
	constructor(id,n){
		super(id);
		this.cn = n;
		const sz = this.size;
		this.csz = [sz[0]/this.cn[0], sz[1]/this.cn[1]];
	}
	resize(sz){
		super.resize(sz);
		const nsz = this.size;
		this.csz = [nsz[0]/this.cn[0], nsz[1]/this.cn[1]];
	}

	//cell from coordinates
	crd_e(c){return this.crd_c(this.crd(c))}
	//cell from internal coordinates
	crd_c(c){return [Math.floor(c[0]/this.csz[0]), Math.floor(c[1]/this.csz[1])]}

	del(cr){
		this.c.clearRect(this.csz[0]*cr[0], this.csz[1]*cr[1], this.csz[0], this.csz[1]);
	}
	pct(cr,p){
		this.draw(p, [this.csz[0]*cr[0], this.csz[1]*cr[1]], [this.csz[0], this.csz[1]]);
	}
	set_clr(c){
		this.c.fillStyle = c;
	}
	fll(cr){
		this.c.fillRect(this.csz[0]*cr[0], this.csz[1]*cr[1], this.csz[0], this.csz[1]);
	}
}
