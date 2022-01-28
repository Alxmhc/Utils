class byteReader{
	constructor(d){
		this.data = Uint8Array.from(d);
		this.pos = 0;
	}
	get(){
		if(this.pos + 1 > this.data.length)
			return null;
		const r = this.data[this.pos];
		this.pos++;
		return r;
	}
	readN(n){
		if(this.pos + n > this.data.length)
			return null;
		this.pos += n;
		return this.data.slice(this.pos - n, this.pos);
	}
}

class bitReader{
	constructor(r){
		this.br = r;
		this.o = 0;
		this.v = null;
	}
	readB(n){
		this.o = 0;
		return this.br.readN(n);
	}
}

//01234567
class bitReaderR extends bitReader{
	read1(){
		if(this.o != 0){
			const r = (this.v >> (7 - this.o)) & 1;
			this.o = (this.o + 1) & 7;
			return r;
		}
		this.v = this.br.get();
		this.o = 1;
		return this.v >> 7;
	}

	readLE(n){
		let rs = 0;
		for(let i = 0; i < n; ++i){
			const c = this.read1();
			rs |= c << i;
		}
		return rs;
	}

	readBE(n){
		let rs = 0;
		for(let i = 0; i < n; ++i){
			const c = this.read1();
			rs = (rs<<1) | c;
		}
		return rs;
	}
}

//76543210
class bitReaderL extends bitReader{
	read1(){
		if(this.o != 0){
			const r = (this.v >> this.o) & 1;
			this.o = (this.o + 1) & 7;
			return r;
		}
		this.v = this.br.get();
		this.o = 1;
		return this.v & 1;
	}

	readLE(n){
		let rs = 0;
		for(let i = 0; i < n; ++i){
			const c = this.read1();
			rs = (rs<<1) | c;
		}
		return rs;
	}

	readBE(n){
		let rs = 0;
		for(let i = 0; i < n; ++i){
			const c = this.read1();
			rs |= c << i;
		}
		return rs;
	}
}
