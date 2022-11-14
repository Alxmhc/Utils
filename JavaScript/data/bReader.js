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
