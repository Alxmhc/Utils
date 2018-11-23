_v = {
	qlen: function(v){return v.dot(v)},
	len: function(v){return Math.sqrt(this.qlen(v))},
	nrm_s: function(v){return v.mul(1.0/this.len(v))},
	nrm: function(v){return this.nrm_s(v.clone())},
	qdst: function(a,b){return this.qlen(a.clone().sub(b))},
	dst: function(a,b){return Math.sqrt(this.qdst(a,b))}
}
