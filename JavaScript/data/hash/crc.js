//rfc1952
class CRC32{
	constructor(){
		this.crc = 0xffffffff;
		this.tbl = CRC32.getTable(0xedb88320);
	}
	get hsize(){
		return 4;
	}
	Update(v){
		for(let i = 0; i < v.length; i++){
			this.crc = this.tbl[(this.crc & 0xff) ^ v[i]] ^ (this.crc >>> 8);
		}
	}
	Final(){
		const r = conv.unpack_le([this.crc ^ 0xffffffff]);
		this.crc = 0xffffffff;
		return r;
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
