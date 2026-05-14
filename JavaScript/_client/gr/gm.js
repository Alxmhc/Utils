class game_field{
	constructor(fld, im){
		this.f = fld;
		this.im = im.slice();
	}
	crd(cr){
		return this.f.crd_c(cr);
	}

	clear_all(){
		this.f.clear_all();
	}
	draw(cr, n){
		this.f.clear_c(cr);
		if(n === null)
			return;
		const o = this.im[n];
		if(o instanceof HTMLImageElement){
			this.f.draw_c(cr, o);
		}
		else if(typeof o === "string"){
			this.f.set_clr(o);
			this.f.fill_c(cr);
		}
	}

	set_arr(arr){
		this.clear_all();
		let k = 0;
		for(let y = 0; y < this.f.cny; ++y){
			for(let x = 0; x < this.f.cnx; ++x){
				this.draw([x,y], arr[k]);
				k++;
			}
		}
	}
	set_pos(p){
		this.clear_all();
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
