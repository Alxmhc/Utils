function $(id){
	return document.getElementById(id);
}

function imgs(p){
	return p.map(e => {
		const im = new Image();
		im.src = e;
		return im;
	});
}

class sw{
	constructor(on, off){
		this._on = function(){on(this.o); this.is_on = true};
		this._off = function(){off(this.o); this.is_on = false};
		this.is_on = false;
	}
	on(){if(!this.is_on){this._on()}}
	off(){if(this.is_on){this._off()}}
	ch(){this.is_on ? this._off() : this._on()}
	sw_other(o){
		if(o !== this.o){
			this.off();
			this.o = o;
			this._on();
			return false;
		}
		if(this.is_on)
			return true;
		this._on();
		return false;
	}
	sw_all(o){
		if(this.sw_other(o))
			this._off();
	}
}
sw.cr = function(on, off, e){
	const r = new sw(on, off);
	r.o = e;
	return r;
}

const $dom = {}

$dom.add_el = function(e, el){
	const s = document.createElement(el);
	e.appendChild(s);
	return s;
}

$dom.mk_list = function(e,m){
	for(let i=0; i<m.length; i++){
		const o = $dom.add_el(e, 'li');
		o.innerHTML = m[i];
	}
}

$dom.mk_select = function(e,d){
	for(let k in d){
		const o = $dom.add_el(e, 'option');
		o.value = k;
		o.text = d[k];
	}
}
