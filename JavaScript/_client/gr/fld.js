class fld_2d extends dom_el{
	constructor(id){
		super(id);
		this.c = this.o.getContext("2d");
	}
	clear(){
		this.c.clearRect(0,0,this.o.width,this.o.height);
	}
	draw(im, crd, sz){
		imgs.fn(im, img => this.c.drawImage(img, crd[0], crd[1], sz[0], sz[1]));
	}
}
