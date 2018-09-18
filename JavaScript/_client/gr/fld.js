class fld_2d{
	constructor(id){
		let o = document.getElementById(id);
		this.c = o.getContext("2d");
		this.w = o.width; this.h = o.height;
		this.dx = o.offsetLeft; this.dy = o.offsetTop;
	}
	crd(c){return [c[0]-this.dx,c[1]-this.dy]}
	clear(){this.c.clearRect(0,0,this.w,this.h)}
	fill(){this.c.fillRect(0,0,this.w,this.h)}
}
