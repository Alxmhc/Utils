//rfc1952
class CRC32{
	get hsize(){return 4;}
	get bsize(){return 1;}
	#Init(){
		this.crc = 0xffffffff;
	}
	constructor(){
		this.tbl = CRC32.getTable(0xedb88320);
		this.#Init();
	}
	Update(v){
		for(let i = 0; i < v.length; i++){
			this.crc = this.tbl[(this.crc ^ v[i]) & 0xff] ^ (this.crc >>> 8);
		}
	}
	Final(){
		const res = conv.unpack4LE(this.crc ^ 0xffffffff);
		this.#Init();
		return res;
	}
}

CRC32.getTable = function(I){
	let tbl = new Uint32Array(256);
	for(let n = 0; n < 256; n++){
		let c = n;
		for(let k = 0; k < 8; k++){
			if((c & 1) != 0){
				c = (c >>> 1) ^ I;
			}
			else{
				c >>>= 1;
			}
		}
		tbl[n] = c;
	}
	return tbl;
}
