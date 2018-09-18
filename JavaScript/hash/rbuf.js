function rbuf(sz){this.buf = new Uint8Array(sz)}
rbuf.prototype = {
clear: function(){this.offset=0},
process: function(v, c){
	const sz = v.length;
	if(this.offset + sz < this.buf.length)
	{
		this.buf.set(v, this.offset);
		this.offset += sz;
		return;
	}
	let part = 0;
	if(this.offset != 0)
	{
		part = this.buf.length - this.offset;
		this.buf.set(v.slice(0, part), this.offset);
		c.process_block(this.buf);
	}
	for(; part + this.buf.length <= sz; part += this.buf.length)
	{
		c.process_block( v.slice(part, part+this.buf.length) );
	}
	this.offset = sz - part;
	if(this.offset != 0)
	{
		this.buf.set( v.slice(part, sz) );
	}
},
sz_e: function(){return this.buf.length - this.offset},
nul: function(){this.buf.fill(0, this.offset)},
push: function(c){this.buf[this.offset]=c;this.offset++}
}
