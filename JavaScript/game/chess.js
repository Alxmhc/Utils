class Chess{
	color = ['white', 'black']
	type = ['pawn', 'rook', 'knight', 'bishop', 'queen', 'king']
	fen_dict = 'PRNBQKprnbqk'

	constructor(p){
		if(p){
			this.arr = p.slice()
		}
		else {
			this.arr = new Array(8*8);
			this.arr.fill(null);
		}
	}
	get pos(){return this.arr.slice()}
	set pos(p){this.arr = p.slice()}
	get fen(){
		let res = "", k = 0;
		for(let y=0; y<8; ++y){
			if(y != 0){
				res += '/';
			}
			let t = 0;
			for(let x=0; x<8; ++x){
				const c = this.arr[k];
				k++;
				if(c === null){
					t++;
					continue;
				}
				if(t != 0){
					res += t;
					t = 0;
				}
				res += this.fen_dict[c];
			}
			if(t != 0){
				res += t;
			}
		}
		return res;
	}
	set fen(s){
		this.arr.fill(null);
		let k = 0;
		for(let i=0, l=s.length; i<l; ++i){
			const c = s[i];
			if(c === '/')
				continue;
			const e = this.fen_dict.indexOf(c);
			if(e === -1){
				k += parseInt(c, 10);
				continue;
			}
			this.arr[k] = e;
			k++;
		}
	}

	get_p(cr){
		return this.arr[cr[0] + (cr[1]<<3)];
	}
	set_p(cr, p){
		this.arr[cr[0] + (cr[1]<<3)] = p;
	}

	moves(m){
		for(let v of m){
			this.set_p(v[0], v[1]);
		}
	}

	_crd1(s){
		const x = {'A':0,'B':1,'C':2,'D':3,'E':4,'F':5,'G':6,'H':7,'a':0,'b':1,'c':2,'d':3,'e':4,'f':5,'g':6,'h':7}[s[0]];
		const y = {'1':7,'2':6,'3':5,'4':4,'5':3,'6':2,'7':1,'8':0}[s[1]];
		return [x, y];
	}
}
Chess.sz = [8,8]
Chess.get_color = function(p){
	if(p === null)
		return null;
	return p < 6 ? 0 : 1;
}
Chess.get_type = function(p){
	if(p === null)
		return null;
	return p < 6 ? p : p - 6;
}
