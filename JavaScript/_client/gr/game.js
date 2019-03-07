function game_field(fld, im){
	this.f = fld;
	this.im = im.slice();
	this.im.unshift(null);
}
game_field.prototype = {
	crd: function(cr){
		return this.f.crd_e(cr);
	},

	clear: function(){
		this.f.clear();
	},
	draw: function(cr, n){
		const o = this.im[n];
		if(!o){
			this.f.del(cr);
		}
		else if(o instanceof HTMLImageElement){
			this.f.del(cr);
			this.f.pct(cr, o);
		}
		else{
			this.f.fll(cr, o);
		}
	},
	set_pos: function(p){
		this.clear();
		this.moves(p);
	},
	//m - [[cr,o],...]
	moves: function(m){
		for(let v of m){
			this.draw(v[0], v[1]);
		}
	}
}
