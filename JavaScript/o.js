class object_pool{
	constructor(f){
		this.create = f;
		this.o = [];
		this.count = 0;
	}
	init(){
		this.count = 0;
	}
	get_object(){
		const n = this.count;
		this.count++;
		if(n < this.o.length)
			return this.o[n];
		let r = this.create();
		this.o.push(r);
		return r;
	}
}
