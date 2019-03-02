class f_sqr extends fld_2d{
	constructor(id,n){
		super(id);
		this.x = n[0];
		this.y = n[1];
		this._set_cell_size();
	}
	_set_cell_size(){
		this.cw = this.o.width/this.x;
		this.ch = this.o.height/this.y;
	}
	resize(x,y){
		super.resize(x,y);
		this._set_cell_size();
	}

	//cell from coordinates
	crd_e(c){return this.crd_c(this.crd(c))}
	//cell from internal coordinates
	crd_c(c){return [Math.floor(c[0]/this.cw), Math.floor(c[1]/this.ch)]}
	crd1_2(c){const e=c%this.x; return [e, (c-e)/this.x]}
	crd2_1(c){return c[1]*this.x + c[0]}

	_pct(cr,p){this.c.drawImage(p, this.cw*cr[0], this.ch*cr[1], this.cw, this.ch)}
	_fll(cr){this.c.fillRect(this.cw*cr[0], this.ch*cr[1], this.cw, this.ch)}

	del(cr){
		this.c.clearRect(this.cw*cr[0], this.ch*cr[1], this.cw, this.ch);
	}
	pct(cr,p){
		if(p.complete){
			this._pct(cr,p);
		}
		else{
			p.addEventListener('load', () => this._pct(cr,p), {once: true});
		}
	}
	fll(cr, p){
		this.c.fillStyle = p;
		this._fll(cr);
	}

	f_f(cl){
		this.c.fillStyle = cl[0];
		this.fill();
		this.c.fillStyle = cl[1];
		for(let i=0; i<this.x; ++i){
			for(let j=0; j<this.y; ++j){
				if((i+j)&1){
					this._fll([i,j]);
				}
			}
		}
	}
}
