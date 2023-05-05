class game_field{
	constructor(fld, im, rev = false){
		this.f = fld;
		this.im = im.slice();
		this._rev = rev;
	}
	crd(cr){
		let c = this.f.crd_e(cr);
		this.corr_cr(c);
		return c;
	}
	corr_cr(cr){
		if(this._rev){
			cr[0] = this.f.csx - 1 - cr[0];
			cr[1] = this.f.csy - 1 - cr[1];
		}
	}

	clear(){
		this.f.clear();
	}
	draw(cr, n){
		this.corr_cr(cr);
		this.f.del(cr);
		if(n === null)
			return;
		const o = this.im[n];
		if(o instanceof HTMLImageElement){
			this.f.pct(cr, o);
		}
		else if(typeof o === "string"){
			this.f.set_clr(o);
			this.f.fll(cr);
		}
	}

	set_arr(arr){
		this.clear();
		let k = 0;
		for(let y = 0; y < this.f.csy; ++y){
			for(let x = 0; x < this.f.csx; ++x){
				this.draw([x,y], arr[k]);
				k++;
			}
		}
	}
	set_pos(p){
		this.clear();
		for(let k in p){
			for(let cr of p[k]){
				this.draw(cr, k);
			}
		}
	}

	//m - [[cr,o],...]
	moves(m){
		for(let v of m){
			this.draw(v[0], v[1]);
		}
	}
}
