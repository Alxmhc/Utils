//rfc1952
class crc32{
	constructor(){
		this.crc = 0xffffffff;
		this.tbl = new Uint32Array(256);
		for(let n = 0; n < 256; n++){
			let c = n;
			for(let k = 0; k < 8; k++){
				if((c & 1) != 0){
					c = (c >>> 1) ^ 0xedb88320;
				}
				else{
					c >>>= 1;
				}
			}
			this.tbl[n] = c;
		}
	}
	Update(v){
		for(let i = 0; i < v.length; i++){
			this.crc = this.tbl[(this.crc & 0xff) ^ v[i]] ^ (this.crc >>> 8);
		}
	}
	Final(){
		const res = this.crc ^ 0xffffffff;
		this.crc = 0xffffffff;
		return res >>> 0;
	}
}
