class vn2{
	constructor(sz,m){
		this.e0 = sz[0];
		this.e1 = sz[1];
		if(m === undefined){
			this.arr = new Array(sz[0]*sz[1]);
			this.arr.fill(0);
		}
		else{
			this.arr = m.slice();
		}
	}
	clone(){return new vn2([this.e0, this.e1], this.arr);}
	_c(c){return c[0]+this.e0*c[1];}
	get(c){return this.arr[this._c(c)];}
	set(c,a){this.arr[this._c(c)]=a;}

	getx(k){
		const c = k * this.e0;
		return this.arr.slice(c, c + this.e0);
	}
	setx(k,x){
		for(let i=0,c=k*this.e0; i<this.e0; ++i){
			this.arr[i+c] = x[i];
		}
	}
	gety(k){
		let r = new Array(this.e1);
		for(let i=k,e=0; e<this.e1; ++e){
			r[e] = this.arr[i];
			i += this.e0;
		}
		return r;
	}
	sety(k,y){
		for(let i=k,e=0; e<this.e1; ++e){
			this.arr[i] = y[e];
			i += this.e0;
		}
	}
	// down-right; k - shift up
	diag1(k = 0){
		let x, y;
		[x, y] = k < 0 ? [0, -k] : [k, 0];
		let r = [];
		for(let c = y*this.e0 + x; (x<this.e0)&&(y<this.e1); c += (this.e0 + 1)){
			r.push(this.arr[c]);
			x++; y++;
		}
		return r
	}
	// down-left; k - shift up
	diag2(k = 0){
		let x, y;
		[x, y] = k < 0 ? [this.e0-1, -k] : [this.e0-1-k, 0];
		let r = [];
		for(let c = y*this.e0 + x; (x>=0)&&(y<this.e1); c += (this.e0 - 1)){
			r.push(this.arr[c]);
			x--; y++;
		}
		return r
	}
}
vn2.from_pos = function(p, sz){
	let res = new vn2(sz);
	for(let k in p){
		for(let cr of p[k]){
			res.set(cr, k);
		}
	}
	return res;
}
