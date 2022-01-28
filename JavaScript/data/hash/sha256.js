class SHA256{
	constructor(){
		this.b = new rbuf(64);
		this.st = new Uint32Array(8);
	}
	get bsize(){
		return 64;
	}
	get hsize(){
		return 32;
	}
	process_block(buf){
		const x = conv.pack4_be(buf);
		this.Transform(x);
	}
	Transform(x){
		let wt = this.st.slice();
		let w = new Uint32Array(64);
		for(let i=0; i<16; i++){
			w[i] = x[i];

			SHA256.rotate(wt, x[i]+SHA256.K[i]);
		}
		for(let i = 16; i<w.length; i++){
			w[i] = w[i-16] + w[i-7];
			w[i] += rotr(w[i-15], 7)^rotr(w[i-15], 18)^(w[i-15]>>>3);
			w[i] += rotr(w[i-2], 17)^rotr(w[i-2], 19)^(w[i-2]>>>10);

			SHA256.rotate(wt, w[i]+SHA256.K[i]);
		}

		this.st[0] += wt[0];
		this.st[1] += wt[1];
		this.st[2] += wt[2];
		this.st[3] += wt[3];
		this.st[4] += wt[4];
		this.st[5] += wt[5];
		this.st[6] += wt[6];
		this.st[7] += wt[7];
	}

	Init(){
		this.st.set([0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19]);
		this.size = 0;
	}
	Update(v){
		this.size += v.length;
		this.b.process(v, this);
	}
	Final(){
		this.b.push(0x80);
		this.b.nul();
		let x = conv.pack4_be(this.b.buf);
		if(this.b.sz_e() < 8){
			this.Transform(x);
			x.fill(0);
		}
		this.b.clear();
		x[14] = this.size >>> 29;
		x[15] = this.size << 3;
		this.Transform(x);
		return conv.unpack4_be(this.st);
	}
}
SHA256.K = new Uint32Array([
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
])
SHA256.rotate = function(wt, t){
	t += wt[7];
	t += rotr(wt[4], 6)^rotr(wt[4], 11)^rotr(wt[4], 25);
	t += (wt[4] & wt[5])^(~wt[4] & wt[6]);
	let tmp = rotr(wt[0], 2)^rotr(wt[0], 13)^rotr(wt[0], 22);
	tmp += (wt[0] & wt[1])|(wt[2] & (wt[0]|wt[1]));
	wt[7] = wt[6];
	wt[6] = wt[5];
	wt[5] = wt[4];
	wt[4] = wt[3] + t;
	wt[3] = wt[2];
	wt[2] = wt[1];
	wt[1] = wt[0];
	wt[0] = t + tmp;
}
