function imgs(p){
	return p.map(e => {
		const im = new Image();
		im.src = e;
		return im;
	});
}

imgs.fn = function(img, fn){
	if(im.complete){
		fn(img);
	}
	else {
		im.addEventListener('load', () => fn(img), {once: true});
	}
}

const $dom = {}

$dom.get_el = function(id){
	return document.getElementById(id);
}

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

class dom_el{
	constructor(id){
		this.o = $dom.get_el(id);
	}
	crd(c){
		return [c[0]-this.o.offsetLeft, c[1]-this.o.offsetTop];
	}
	get size(){
		return [this.o.width, this.o.height];
	}
	resize(sz){
		if(sz[0]){
			this.o.width  = sz[0];
		}
		if(sz[1]){
			this.o.height = sz[1];
		}
	}
}
