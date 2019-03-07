class vn2 extends Array{
	constructor(n,m){
		if(m === undefined){
			m = 1
		}
		super(n*m);
		this.sx = n;
		this.sy = m;
	}
	clone(){return new vn2(this.sx, this.sy);}
	_c(c){return c[0]+this.sx*c[1];}
	get(c){return this[this._c(c)];}
	set(c,a){this[this._c(c)]=a;}

	getx(k){
		let r = new Array(this.sx);
		for(let i=0; i<this.sx; ++i){
			r[i] = this[i+k*this.sx];
		}
		return r;
	}
	setx(k,x){
		for(let i=0; i<this.sx; ++i){
			this[i+k*this.sx] = x[i];
		}
	}
	gety(k){
		let r = new Array(this.sy);
		for(let i=k,e=0; e<this.sy; ++e){
			r[e] = this[i];
			i += this.sx;
		}
		return r;
	}
	sety(k,y){
		for(let i=k,e=0; e<this.sy; ++e){
			this[i] = y[e];
			i += this.sx;
		}
	}
	// down-right; k - shift up
	diag1(k = 0){
		let x, y;
		[x, y] = k < 0 ? [0, -k] : [k, 0];
		let r = [];
		for(let c = y*this.sx + x; (x<this.sx)&&(y<this.sy); c += (this.sx + 1)){
			r.push(this[c]);
			x++; y++;
		}
		return r
	}
	// down-left; k - shift up
	diag2(k = 0){
		let x, y;
		[x, y] = k < 0 ? [this.sx-1, -k] : [this.sx-1-k, 0];
		let r = [];
		for(let c = y*this.sx + x; (x>=0)&&(y<this.sy); c += (this.sx - 1)){
			r.push(this[c]);
			x--; y++;
		}
		return r
	}
}
