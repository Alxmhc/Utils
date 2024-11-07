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
	swap(a, i, j){
		const t = a[i]
		a[i] = a[j]
		a[j] = t
	}
}

function nxt_perm(a){
	if(a.length < 2)
		return false
	const li = a.length-1
	let i = li
	while(i!=0){
		if(a[i] > a[i-1])
			break
		i--;
	}
	if(i == 0)
		return false

	for(let p=0,k=(li+1-i)>>1; p<k; p++){
		_arr.swap(a, i+p, li-p)
	}

	let j = i
	i--
	while(a[i]>=a[j]){
		j++
	}
	_arr.swap(a, i, j)

	return true
}
