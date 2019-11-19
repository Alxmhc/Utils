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
			cr[0] = this.f.sx - 1 - cr[0];
			cr[1] = this.f.sy - 1 - cr[1];
		}
	}

	clear(){
		this.f.clear();
	}
	draw(cr, n){
		this.corr_cr(cr);
		if(n === null){
			this.f.del(cr);
			return;
		}
		const o = this.im[n];
		if(o instanceof HTMLImageElement){
			this.f.del(cr);
			this.f.pct(cr, o);
		}
		else if(typeof o === "string"){
			this.f.fll(cr, o);
		}
	}
	set_arr(arr){
		this.clear();
		let k = 0;
		for(let y = 0; y < this.f.sy; ++y){
			for(let x = 0; x < this.f.sx; ++x){
				const c = arr[k];
				k++;
				if(c === null)
					continue;
				this.draw([x,y], c);
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
