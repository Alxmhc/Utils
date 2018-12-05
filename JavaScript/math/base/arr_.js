_arr = {
	sum: function(v){
		let s = 0;
		const n = v.length;
		for(let i=0; i<n; ++i){s+=v[i];}
		return s;
	},
	avr: function(v){
		const n = v.length;
		return n!==0 ? _arr.sum(v)/n : 0;
	},
	dot: function(v1, v2){
		let s = 0;
		const n = v1.length;
		for(let i=0; i<n; ++i){s+=v1[i]*v2[i];}
		return s;
	}
}
