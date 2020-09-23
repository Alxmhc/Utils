class f_sqr extends fld_2d{
	constructor(id,n){
		super(id);
		this.sx = n[0];
		this.sy = n[1];
		this._set_cell_size();
	}
	_set_cell_size(){
		this.cw = this.o.width/this.sx;
		this.ch = this.o.height/this.sy;
	}
	resize(x,y){
		super.resize(x,y);
		this._set_cell_size();
	}

	//cell from coordinates
	crd_e(c){return this.crd_c(this.crd(c))}
	//cell from internal coordinates
	crd_c(c){return [Math.floor(c[0]/this.cw), Math.floor(c[1]/this.ch)]}

	del(cr){
		this.c.clearRect(this.cw*cr[0], this.ch*cr[1], this.cw, this.ch);
	}
	_pct(cr,p){this.c.drawImage(p, this.cw*cr[0], this.ch*cr[1], this.cw, this.ch)}
	pct(cr,p){
		if(p.complete){
			this._pct(cr,p);
		}
		else{
			p.addEventListener('load', () => this._pct(cr,p), {once: true});
		}
	}
	set_clr(c){this.c.fillStyle = c;}
	fll(cr){this.c.fillRect(this.cw*cr[0], this.ch*cr[1], this.cw, this.ch)}
}

function f_f(fld, c){
	fld.set_clr(c[0]);
	fld.fill();
	fld.set_clr(c[1]);
	for(let i=0; i<fld.sx; ++i){
		for(let j=0; j<fld.sy; ++j){
			if((i+j)&1){
				fld.fll([i,j]);
			}
		}
	}
}
