class fld_2d{
	constructor(id){
		this.o = document.getElementById(id);
		this.c = this.o.getContext("2d");
	}
	crd(c){
		return [c[0]-this.o.offsetLeft,c[1]-this.o.offsetTop];
	}
	clear(){
		this.c.clearRect(0,0,this.o.width,this.o.height);
	}
	fill(){
		this.c.fillRect(0,0,this.o.width,this.o.height);
	}
	resize(x,y){
		if(x){this.o.width = x;}
		if(y){this.o.height = y;}
	}
	get size(){
		return [this.o.width, this.o.height];
	}
}
