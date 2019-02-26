function i64(l,h){
	this.l = l>>>0;
	this.h = h>>>0;
}

i64.prototype = {
clone: function(){
	return new i64(this.l,this.h);
},
copy: function(c){
	this.l = c.l;
	this.h = c.h;
	return this;
},
shl: function(n){
	if(n > 31){
		this.h = n<64 ? this.l<<(n&31) : 0;
		this.l = 0;
	}
	else if(n != 0){
		this.h = (this.h<<n)|(this.l>>>(32-n));
		this.l <<= n;
	}
	return this;
},
shr: function(n){
	if(n > 31){
		this.l = n<64 ? this.h>>>(n&31) : 0;
		this.h = 0;
	}
	else if(n != 0){
		this.l = (this.l>>>n)|(this.h<<(32-n));
		this.h >>>= n;
	}
	return this;
},
rotl: function(n){
	const k = n&31;
	if(k != 0){
		const t = (this.h<<k)|(this.l>>>(32-k));
		this.l = (this.l<<k)|(this.h>>>(32-k));
		this.h = t;
	}
	if((n&32) != 0){
		[this.l,this.h] = [this.h,this.l];
	}
	return this;
},
rotr: function(n){
	const k = n&31;
	if(k != 0){
		const t = (this.h>>>k)|(this.l<<(32-k));
		this.l = (this.l>>>k)|(this.h<<(32-k));
		this.h = t;
	}
	if((n&32) != 0){
		[this.l,this.h] = [this.h,this.l];
	}
	return this;
},
xor: function(c){
	this.l ^= c.l;
	this.h ^= c.h;
	return this;
},

addc: function(c){
	this.l = (this.l+c)>>>0;
	if(this.l < (c>>>0)){
		this.h = (this.h+1)>>>0;
	}
	return this;
},
mulc: function(c){
	const ah=this.l>>>16, bh=c>>>16, al=this.l&0xffff, bl=c&0xffff;
	let x,r=new i64(al*bl,ah*bh);
	x=ah*bl; if(x){r.h+=x>>16; r.addc(x<<16);}
	x=bh*al; if(x){r.h+=x>>16; r.addc(x<<16);}
	r.h = (r.h + this.h*c)>>>0;
	return this.copy(r);
},
add: function(c){
	this.h += c.h;
	return this.addc(c.l);
}
}
i64.add = function(a,b){let r=new i64(a); return r.addc(b);}
i64.mul = function(a,b){let r=new i64(a); return r.mulc(b);}
