class f_sqr extends fld_2d{
	#csx;
	#csy;
	#resz(){
		this.#csx = this.szx/this.cnx;
		this.#csy = this.szy/this.cny;
	}

	constructor(id,n){
		super(id);
		this.cnx = n[0];
		this.cny = n[1];
		this.#resz();
	}
	resize(sz){
		super.resize(sz);
		this.#resz();
	}

	//cell from internal coordinates
	crdi_c(c){
		return [Math.floor(c[0]/this.#csx), Math.floor(c[1]/this.#csy)];
	}
	//cell from coordinates
	crd_c(c){
		const icr = this.crd(c);
		return this.crdi_c(icr);
	}

	clear_c(cr){
		this.clear(this.#csx*cr[0], this.#csy*cr[1], this.#csx, this.#csy);
	}
	fill_c(cr){
		this.fill(this.#csx*cr[0], this.#csy*cr[1], this.#csx, this.#csy);
	}
	draw_c(cr,p){
		this.draw(p, this.#csx*cr[0], this.#csy*cr[1], this.#csx, this.#csy);
	}
}
