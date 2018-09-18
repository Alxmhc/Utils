function vn2(n,m){this.m = new Array(n*m);this.sx=n;this.sy=m}

vn2.prototype = {
_c: function(c){return c[0]+this.sx*c[1]},
nul: function(){this.m.fill(0)},
get2: function(c){return this.m[this._c(c)]},
set2: function(c,a){this.m[this._c(c)]=a},
getx: function(k){return this.m.slice(k*this.sx,(k+1)*this.sx)},
gety: function(k){let r=new Array(this.sy);for(let i=k,e=0; e<this.sy; e++){r[e]=this.m[i];i+=this.sx}return r},
// down-right; k - shift up
diag1: function(k = 0){
	let x, y;
	[x, y] = k < 0 ? [0, -k] : [k, 0];
	let r = [];
	for(let c = y*this.sx + x; (x<this.sx)&&(y<this.sy); c += (this.sx + 1)){
		r.push(this.m[c]);
		x++; y++;
	}
	return r
},
// down-left; k - shift up
diag2: function(k = 0){
	let x, y;
	[x, y] = k < 0 ? [this.sx-1, -k] : [this.sx-1-k, 0];
	let r = [];
	for(let c = y*this.sx + x; (x>=0)&&(y<this.sy); c += (this.sx - 1)){
		r.push(this.m[c]);
		x--; y++;
	}
	return r
}
}
