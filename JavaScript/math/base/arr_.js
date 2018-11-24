_arr = {
	sum: function(m){
		let s = 0;
		const n = m.length;
		for(let i=0; i<n; ++i){s+=m[i];}
		return s;
	},
	avr: function(m){
		const n = m.length;
		return n!==0 ? _arr.sum(m)/n : 0;
	},
	dot: function(v1, v2){
		let s = 0;
		const n = v1.length;
		for(let i=0; i<n; ++i){s+=v1[i]*v2[i];}
		return s;
	}
}
