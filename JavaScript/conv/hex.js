hex = {
	encode: function(m)
	{
		let s = "";
		for(let i=0; i<m.length; i++){
			s += hex.s[m[i]>>>4] + hex.s[m[i]&0x0f];
		}
		return s;
	},
	decode: function(s)
	{
		const sz = s.length;
		let r = new Uint8Array(sz>>>1);
		for(let i=0,j=0; i<sz; i+=2){
			r[j++] = parseInt(s.substring(i,i+2),16);
		}
		return r;
	}
}
hex.s = "0123456789abcdef"
