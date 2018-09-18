conv = {
	pack_le: function(m)
	{
		const sz = m.length;
		let r = new Uint32Array(sz>>>2);
		for(let i=0,j=0; i<sz; i+=4){
			r[j++] = (m[i+3]<<24)|(m[i+2]<<16)|(m[i+1]<<8)|m[i];
		}
		return r;
	},
	pack_be: function(m)
	{
		const sz = m.length;
		let r = new Uint32Array(sz>>>2);
		for(let i=0,j=0; i<sz; i+=4){
			r[j++] = (m[i]<<24)|(m[i+1]<<16)|(m[i+2]<<8)|m[i+3];
		}
		return r;
	},
	unpack_le: function(m)
	{
		const sz = m.length;
		let r = new Uint8Array(sz<<2);
		for(let i=0; i<sz; i++){
			r.set([m[i]&0xff,(m[i]>>>8)&0xff,(m[i]>>>16)&0xff,m[i]>>>24], i<<2);
		}
		return r;
	},
	unpack_be: function(m)
	{
		const sz = m.length;
		let r = new Uint8Array(sz<<2);
		for(let i=0; i<sz; i++){
			r.set([m[i]>>>24,(m[i]>>>16)&0xff,(m[i]>>>8)&0xff,m[i]&0xff], i<<2);
		}
		return r;
	}
}
