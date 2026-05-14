class fld_2d extends dom_el{
	#c;
	constructor(id){
		super(id);
		this.#c = this.o.getContext("2d");
	}
	set_clr(c){
		this.#c.fillStyle = c;
	}
	clear(crx, cry, szx, szy){
		this.#c.clearRect(crx, cry, szx, szy);
	}
	fill(crx, cry, szx, szy){
		this.#c.fillRect(crx, cry, szx, szy);
	}
	draw(im, crx, cry, szx, szy){
		img.fn(im, p => this.#c.drawImage(p, crx, cry, szx, szy));
	}

	clear_all(){
		this.clear(0, 0, this.szx, this.szy);
	}
	fill_all(){
		this.fill(0, 0, this.szx, this.szy);
	}
}
