conv = {
	pack2LE: function(a){return (a[1]<<8)|a[0]},
	pack2BE: function(a){return (a[0]<<8)|a[1]},
	pack4LE: function(a){return (a[3]<<24)|(a[2]<<16)|(a[1]<<8)|a[0]},
	pack4BE: function(a){return (a[0]<<24)|(a[1]<<16)|(a[2]<<8)|a[3]},

	unpack4LE: function(c){return Uint8Array.from([c,c>>>8,c>>>16,c>>>24])},
	unpack4BE: function(c){return Uint8Array.from([c>>>24,c>>>16,c>>>8,c])},

	pack4_le: function(m){
		const sz = m.length;
		let r = new Uint32Array(sz>>>2);
		for(let i=0,j=0; i<sz; i+=4){
			r[j++] = conv.pack4LE(m.slice(i, i+4));
		}
		return r;
	},
	pack4_be: function(m){
		const sz = m.length;
		let r = new Uint32Array(sz>>>2);
		for(let i=0,j=0; i<sz; i+=4){
			r[j++] = conv.pack4BE(m.slice(i, i+4));
		}
		return r;
	},
	unpack4_le: function(m){
		const sz = m.length;
		let r = new Uint8Array(sz<<2);
		for(let i=0; i<sz; i++){
			r.set(conv.unpack4LE(m[i]), i<<2);
		}
		return r;
	},
	unpack4_be: function(m){
		const sz = m.length;
		let r = new Uint8Array(sz<<2);
		for(let i=0; i<sz; i++){
			r.set(conv.unpack4BE(m[i]), i<<2);
		}
		return r;
	}
}
