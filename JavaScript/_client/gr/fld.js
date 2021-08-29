class fld_2d{
	constructor(id){
		this.o = document.getElementById(id);
		this.c = this.o.getContext("2d");
	}
	crd(c){
		return [c[0]-this.o.offsetLeft,c[1]-this.o.offsetTop];
	}
	get size(){
		return [this.o.width, this.o.height];
	}
	resize(sz){
		if(sz[0]){
			this.o.width  = sz[0];
		}
		if(sz[1]){
			this.o.height = sz[1];
		}
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
