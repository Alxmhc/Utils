class fld_2d extends dom_el{
	constructor(id){
		super(id);
		this.c = this.o.getContext("2d");
	}
	clear(){
		this.c.clearRect(0,0,this.o.width,this.o.height);
	}
	set_clr(c){
		this.c.fillStyle = c;
	}
	draw(im, crd, sz){
		img.fn(im, p => this.c.drawImage(p, crd[0], crd[1], sz[0], sz[1]));
	}
	fill(){
		this.c.fillRect(0, 0, this.szx, this.szy);
	}
}
