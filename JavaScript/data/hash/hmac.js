class HMAC{
	get hsize(){return this.h.hsize;}
	get bsize(){return this.h.bsize;}
	#Init(){
		this.h.Update(this.ki);
	}
	constructor(hash, key){
		this.h = (typeof(hash) == "object") ? hash : new hash;
		this.ki = new Uint8Array(this.h.bsize);
		this.ko = new Uint8Array(this.h.bsize);
		this.ki.fill(0x36);
		this.ko.fill(0x5c);
		if(key.length > this.h.bsize){
			this.h.Update(key);
			key = this.h.Final();
		}
		for(let i = 0; i < key.length; i++){
			this.ki[i] ^= key[i];
			this.ko[i] ^= key[i];
		}
		this.#Init();
	}
	Update(v){
		this.h.Update(v);
	}
	Final(){
		let r = this.h.Final();
		this.h.Update(this.ko);
		this.h.Update(r);
		const res = this.h.Final();
		this.#Init();
		return res;
	}
}
