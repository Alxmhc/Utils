function game_field(fld, im){
	this.f = fld;
	this.im = im.slice();
}
game_field.prototype = {
	crd: function(cr){
		return this.f.crd_e(cr);
	},
	clear: function(){
		this.f.clear();
	},
	draw: function(cr, n){
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
	},

	set_pos: function(p){
		this.clear();
		for(let k in p){
			for(let cr of p[k]){
				this.draw(cr, k);
			}
		}
	},
	//m - [[cr,o],...]
	moves: function(m){
		for(let v of m){
			this.draw(v[0], v[1]);
		}
	}
}
