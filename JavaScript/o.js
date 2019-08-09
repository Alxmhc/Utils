class object_pool{
	constructor(f){
		this.create = f;
		this.o = [];
	}
	init(){
		this.count = 0;
	}
	get_object(){
		const n = this.count;
		this.count++;
		if(n == this.o.length){
			let r = this.create();
			this.o.push(r);
			return r;
		}
		return this.o[n];
	}
}
