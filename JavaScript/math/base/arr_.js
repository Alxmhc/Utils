_v = {
	qlen: function(v){
		return v.dot(v)
	},
	len: function(v){
		return Math.sqrt(this.qlen(v))
	},
	qdst: function(a,b){
		return this.qlen(a.clone().sub(b))
	},
	dst: function(a,b){
		return Math.sqrt(this.qdst(a,b))
	},
	nrm: function(v){
		v.mul(1.0/this.len(v))
	}
}

_arr = {
	count: function(v,e){
		let k = 0
		for(let i=0, n=v.length; i<n; ++i){
			if(v[i] == e){
				k++
			}
		}
		return k
	},
	sum: function(v){
		let s = 0
		for(let i=0, n=v.length; i<n; ++i){
			s += v[i]
		}
		return s
	},
	avr: function(v){
		const n = v.length
		return n!==0 ? this.sum(v)/n : 0
	},

	eq: function(v1, v2){
		let n = v1.length
		if(v2.length != n)
			return false
		while(n--){
			if(v1[n] != v2[n])
				return false
		}
		return true
	},
	dot: function(v1, v2){
		let s = 0
		for(let i=0, n=v1.length; i<n; ++i){
			s += v1[i] * v2[i]
		}
		return s
	},
	s_add: function(v1, v2){
		for(let i=0, n=v1.length; i<n; ++i){
			v1[i] += v2[i]
		}
	},
	s_sub: function(v1, v2){
		for(let i=0, n=v1.length; i<n; ++i){
			v1[i] -= v2[i]
		}
	},
	s_mul_k: function(v, k){
		for(let i=0, n=v.length; i<n; ++i){
			v[i] *= k
		}
	},

	m_chk(v,m){
		for(let i=0, n=v.length; i<n; ++i){
			if(m[i] !== null && v[i] != m[i])
				return false
		}
		return true
	},
	m_all(v,m){
		for(let i=0, n=v.length; i<n; ++i){
			if(v[i] != m[i]){
				v[i] = null
			}
		}
	},
	m_or(v,m){
		for(let i=0, n=v.length; i<n; ++i){
			if(v[i] == null){
				v[i] = m[i]
			}
		}
	}
}
