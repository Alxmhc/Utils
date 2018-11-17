class fld_2d{
	constructor(id){
		this.o = document.getElementById(id);
		this.c = this.o.getContext("2d");
		this.dx = this.o.offsetLeft;
		this.dy = this.o.offsetTop;
	}
	crd(c){return [c[0]-this.dx,c[1]-this.dy]}
	clear(){this.c.clearRect(0,0,this.o.width,this.o.height)}
	fill(){this.c.fillRect(0,0,this.o.width,this.o.height)}
	resize(x,y){
		if(x !== null){this.o.width = x}
		if(y !== null){this.o.height = y}
	}
}
