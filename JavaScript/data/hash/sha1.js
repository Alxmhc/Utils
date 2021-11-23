class SHA1{
	constructor(){
		this.b = new rbuf(64);
		this.st = new Uint32Array(5);
		this.init();
	}
	get hsize(){
		return 20;
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
		w.set(x);
		for(let i = 0; i < 80; i++){
			if(i > 15){
				w[i] = rotl(w[i-3]^w[i-8]^w[i-14]^w[i-16], 1);
			}
			let t = w[i] + rotl(wt[0],5) + wt[4];

			if(i < 20){
				t += ((wt[1] & wt[2])|(~wt[1] & wt[3])) + 0x5a827999;
			}else if(i < 40){
				t += (wt[1] ^ wt[2] ^ wt[3]) + 0x6ed9eba1;
			}else if(i < 60){
				t += ((wt[1] & wt[2])|(wt[3] & (wt[1]|wt[2]))) + 0x8f1bbcdc;
			}else{
				t += (wt[1] ^ wt[2] ^ wt[3]) + 0xca62c1d6;
			}
			wt[4] = wt[3];
			wt[3] = wt[2];
			wt[2] = rotl(wt[1], 30);
			wt[1] = wt[0];
			wt[0] = t;
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
		x[14] = this.size >>> 29;
		x[15] = this.size << 3;
		this.Transform(x);

		const r = conv.unpack_be(this.st);
		this.clear();
		this.init();
		return r;
	}
}
