class fld_2d extends dom_el{
	constructor(id){
		super(id);
		this.c = this.o.getContext("2d");
	}
	clear(){
		this.c.clearRect(0,0,this.o.width,this.o.height);
	}
	draw(im, crd, sz){
		if(im.complete){
			this.c.drawImage(im, crd[0], crd[1], sz[0], sz[1]);
		}
		else{
			im.addEventListener('load', () => this.c.drawImage(im, crd[0], crd[1], sz[0], sz[1]), {once: true});
		}
	}
}
