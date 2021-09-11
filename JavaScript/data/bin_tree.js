class binTree {
	constructor(){
		this.l = null;
		this.r = null;
		this.val = null;
	}
	add(n, nbit, v){
		let f = 1 << (nbit - 1);
		let e = this;
		while(f != 0){
			const c = n & f;
			if(c == 0){
				if(e.l == null){
					e.l = new binTree();
				}
				e = e.l;
			}
			else{
				if(e.r == null){
					e.r = new binTree();
				}
				e = e.r;
			}
			f >>= 1;
		}
		e.val = v;
	}
	find(br){
		let e = this;
		for(;;){
			const b = br.read1();
			e = b == 0 ? e.l : e.r;
			if(e == null)
				return null;
			if(e.val != null)
				return e.val;
		}
	}
}
