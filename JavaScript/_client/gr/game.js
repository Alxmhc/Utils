function game_field(id, sz, im){
	this.f = new f_sqr(id, sz);
	this.im = im.slice();
	this.im.unshift(null);
}
game_field.prototype = {
	clear: function(){this.f.clear();},
	crd: function(cr){return this.f.crd_e(cr);},

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

	set_arr: function(m){
		this.clear();
		for(let i=0,o=0; i<this.f.y; ++i){
			for(let j=0; j<this.f.x; ++j){
				if(m[o]){
					this.draw([j,i],m[o]);
				}
				o++;
			}
		}
	},
	moves: function(m){
		for(let v of m){
			const cr = this.f.crd1_2(v[0]);
			this.draw(cr, v[1]);
		}
	}
}
