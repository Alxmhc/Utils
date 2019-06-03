class SHA1{
	constructor(){
		this.b = new rbuf(64);
		this.st = new Uint32Array(5);
		this.init();
	}
	init(){
		this.st.set([0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xc3d2e1f0]);
		this.size = 0;
	}
	clear(){
		this.b.clear();
	}
	process_block(buf){
		const x = conv.pack_be(buf);
		this.Transform(x);
	}
	Transform(x){
		let wt = this.st.slice();
		let w = new Uint32Array(80);
		for(let i=0; i<16; i++){
			w[i] = x[i];

			SHA1.rotate1(wt, x[i]);
		}
		for(let i = 16; i<w.length; i++){
			w[i] = rotl(w[i-3]^w[i-8]^w[i-14]^w[i-16], 1);

			if(i<20){
				SHA1.rotate1(wt, w[i]);
			}else if(i<40){
				SHA1.rotate2(wt, w[i]);
			}else if(i<60){
				SHA1.rotate3(wt, w[i]);
			}else{
				SHA1.rotate4(wt, w[i]);
			}
		}

		this.st[0] += wt[0];
		this.st[1] += wt[1];
		this.st[2] += wt[2];
		this.st[3] += wt[3];
		this.st[4] += wt[4];
	}
	Update(v){
		this.size += v.length;
		this.b.process(v, this);
	}
	Final(){
		this.b.push(0x80);
		this.b.nul();
		let x = conv.pack_be(this.b.buf);
		if(this.b.sz_e() < 8){
			this.Transform(x);
			x.fill(0);
		}
		x[14] = this.size>>>29;
		x[15] = this.size<<3;
		this.Transform(x);

		const r = conv.unpack_be(this.st);
		this.clear();
		this.init();
		return r;
	}
}
SHA1.rotate = function(wt, t){
	t += rotl(wt[0],5)+wt[4];
	wt[4] = wt[3];
	wt[3] = wt[2];
	wt[2] = rotl(wt[1], 30);
	wt[1] = wt[0];
	wt[0] = t;
}
SHA1.rotate1 = function(wt, t){
	t += ((wt[1] & wt[2])|(~wt[1] & wt[3]))+0x5a827999;
	SHA1.rotate(wt,t);
}
SHA1.rotate2 = function(wt, t){
	t += (wt[1]^wt[2]^wt[3])+0x6ed9eba1;
	SHA1.rotate(wt, t);
}
SHA1.rotate3 = function(wt, t){
	t += ((wt[1] & wt[2])|(wt[3] & (wt[1]|wt[2])))+0x8f1bbcdc;
	SHA1.rotate(wt, t);
}
SHA1.rotate4 = function(wt, t){
	t += (wt[1]^wt[2]^wt[3])+0xca62c1d6;
	SHA1.rotate(wt, t);
}
