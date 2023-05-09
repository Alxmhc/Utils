_v = {
	qlen: function(v){
		return v.dot(v);
	},
	len: function(v){
		return Math.sqrt(this.qlen(v));
	},
	qdst: function(a,b){
		return this.qlen(a.clone().sub(b));
	},
	dst: function(a,b){
		return Math.sqrt(this.qdst(a,b));
	}
}

function nrm(v){
	return v.mul(1.0/_v.len(v))
}

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
		for(let i=0, n=v.length; i<n; ++i){
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
		for(let i=0, n=v1.length; i<n; ++i){
			s += v1[i] * v2[i];
		}
		return s;
	},
	count: function(v,e){
		let k = 0;
		for(let i=0, n=v.length; i<n; ++i){
			if(v[i] == e){
				k++;
			}
		}
		return k;
	},
	v_chk(v,m){
		for(let i=0, n=v.length; i<n; ++i){
			if(m[i] !== null && v[i] != m[i])
				return false;
		}
		return true;
	},
	v_all(v,m){
		for(let i=1, n=v.length; i<n; ++i){
			if(v[i] != m[i]){
				v[i] = null;
			}
		}
	},
	v_or(v,m){
		for(let i=1, n=v.length; i<n; ++i){
			if(v[i] == null){
				v[i] = m[i];
			}
		}
	}
}
