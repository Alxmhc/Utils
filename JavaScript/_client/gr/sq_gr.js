class f_sqr extends fld_2d{
	constructor(id,n){
		super(id);
		this.sx = n[0];
		this.sy = n[1];
		this.cw = this.o.width/this.sx;
		this.ch = this.o.height/this.sy;
	}
	resize(sz){
		super.resize(sz);
		this.cw = this.o.width/this.sx;
		this.ch = this.o.height/this.sy;
	}

	//cell from coordinates
	crd_e(c){return this.crd_c(this.crd(c))}
	//cell from internal coordinates
	crd_c(c){return [Math.floor(c[0]/this.cw), Math.floor(c[1]/this.ch)]}

	del(cr){
		this.c.clearRect(this.cw*cr[0], this.ch*cr[1], this.cw, this.ch);
	}
	pct(cr,p){
		this.draw(p, [this.cw*cr[0], this.ch*cr[1]], [this.cw, this.ch]);
	}
	set_clr(c){
		this.c.fillStyle = c;
	}
	fll(cr){
		this.c.fillRect(this.cw*cr[0], this.ch*cr[1], this.cw, this.ch);
	}
}
