function i64(l,h){this.l=l>>>0;this.h=h>>>0}
i64.prototype = {
clone: function(){return new i64(this.l,this.h)},
copy: function(c){this.h=c.h; this.l=c.l; return this},
shl: function(n){
	if(n){
		let d=this.l<<(n&31)
		if(n<32){this.h=(this.h<<n)|(this.l>>>(32-n));this.l=d}
		else{this.l=0;this.h=n<64?d:0}
	}return this;
},
shr: function(n){
	if(n){
		let d=this.h>>>(n&31)
		if(n<32){this.l=(this.l>>>n)|(this.h<<(32-n));this.h=d}
		else{this.h=0;this.l=n<64?d:0}
	}return this;
},
rotl: function(n){n&=63;let k=n&31
	if(k){
		let a=(this.l<<k)|(this.h>>>(32-k)),b=(this.h<<k)|(this.l>>>(32-k))
		if(n<32){this.l=a;this.h=b}else{this.l=b;this.h=a}
	}else{[this.l,this.h]=[this.h,this.l]}
	return this;
},
rotr: function(n){n&=63;let k=n&31
	if(k){
		let a=(this.l>>>k)|(this.h<<(32-k)),b=(this.h>>>k)|(this.l<<(32-k))
		if(n<32){this.l=a;this.h=b}else{this.l=b;this.h=a}
	}else{[this.l,this.h]=[this.h,this.l]}
	return this;
},

addc: function(c){
	this.l = (this.l+c)>>>0;
	if(this.l<(c>>>0)){
		this.h = (this.h+1)>>>0;
	}
	return this;
},
mulc: function(c){
	let x,ah=this.l>>>16,bh=c>>>16,al=this.l&0xffff,bl=c&0xffff,r=new i64(al*bl,ah*bh)
	x=ah*bl; if(x){r.h+=x>>16; r.addc(x<<16)}
	x=bh*al; if(x){r.h+=x>>16; r.addc(x<<16)}
	if(this.h){r.h = (this.h*c+r.h)>>>0}
	r.h>>>=0; r.l>>>=0;
	return this.copy(r);
},

add: function(c){this.h+=c.h; return this.addc(c.l);},
xor: function(c){this.l^=c.l; this.h^=c.h; return this;}
}
i64.add = function(a,b){let r=new i64(a); return r.addc(b);}
i64.mul = function(a,b){let r=new i64(a); return r.mulc(b);}
