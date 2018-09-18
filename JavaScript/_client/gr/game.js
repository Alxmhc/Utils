function game_client(id, sz, ims){
	this.f = new f_sqr(id, sz);
	this.im = imgs(ims);
	this.im.unshift(null);
}
game_client.prototype = {
clear: function(){this.f.clear();},
crd: function(cr){return this.f.crd_e(cr)},
crd1_2: function(c){return this.f.crd1_2(c)},
crd2_1: function(c){return this.f.crd2_1(c)},

del2: function(c){this.f.del(c)},
set2: function(c, p){this.del2(c); if(p){this.f.pct(c,this.im[p])}},
set_arr: function(m){
	for(let i=0,o=0; i<this.f.y; i++){
		for(let j=0; j<this.f.x; j++){
			this.set2([j,i],m[o]);
			o++;
		}
	}
},
//c = y*sx + x
del: function(c){this.del2(this.crd1_2(c))},
set: function(c, p){this.set2(this.crd1_2(c), p)},
moves: function(m){
	for(let v of m){
		this.set(v[0], v[1]);
	}
}
}
