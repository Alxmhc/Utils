class HMAC{
	constructor(hash, key){
		this.h = hash;
		this.ki = new Uint8Array(hash.bsize);
		this.ko = new Uint8Array(hash.bsize);
		this.SetKey(key);
	}
	get hsize(){
		return hash.hsize;
	}
	SetKey(key){
		this.ki.fill(0x36);
		this.ko.fill(0x5c);
		if(key.length > this.h.bsize){
			this.h.Init();
			this.h.Update(key);
			key = this.h.Final();
		}
		for(let i = 0; i < key.length; i++){
			this.ki[i] ^= key[i];
			this.ko[i] ^= key[i];
		}		
	}

	Init(){
		this.h.Init();
		this.h.Update(this.ki);
	}
	Update(v){
		this.h.Update(v);
	}
	Final(){
		let r = this.h.Final();
		this.h.Init();
		this.h.Update(this.ko);
		this.h.Update(r);
		return this.h.Final();
	}
}
