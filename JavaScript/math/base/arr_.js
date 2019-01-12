_arr = {
	eq: function(a1, a2){
		let n = a1.length;
		if(a2.length != n)
			return false
		while(n--){
			if(a1[n] != a2[n])
				return false
		}
		return true
	},
	sum: function(v){
		let s = 0;
		const n = v.length;
		for(let i=0; i<n; ++i){
			s += v[i];
		}
		return s;
	},
	avr: function(v){
		const n = v.length;
		return n!==0 ? _arr.sum(v)/n : 0;
	},
	dot: function(v1, v2){
		let s = 0;
		const n = v1.length;
		for(let i=0; i<n; ++i){
			s += v1[i]*v2[i];
		}
		return s;
	}
}
