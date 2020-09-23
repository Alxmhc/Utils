Chess = {
	sz: [8,8],
	color: ['white', 'black'],
	type: ['pawn', 'rook', 'knight', 'bishop', 'queen', 'king']
}

Chess.crd = function(s){
	const x = {'A':0,'B':1,'C':2,'D':3,'E':4,'F':5,'G':6,'H':7,'a':0,'b':1,'c':2,'d':3,'e':4,'f':5,'g':6,'h':7}[s[0]];
	const y = {'1':0,'2':1,'3':2,'4':3,'5':4,'6':5,'7':6,'8':7}[s[1]];
	return [x, y];
}

Chess.inf = function(p){
	if(p === null)
		return {color: null, type: null};
	if(p < 6)
		return {color: 0, type: p};
	return {color: 1, type: p - 6};
}

Chess.fen = {
	_dict: 'PRNBQKprnbqk',
	to_pos: function(s){
		let res = {}, x=0, y=0;
		for(let i=0, l=s.length; i<l; ++i){
			const c = s[i];
			if(c === '/'){
				y++;
				x = 0;
				continue;
			}
			const e = this._dict.indexOf(c);
			if(e === -1){
				x += parseInt(c, 10);
				continue;
			}
			const cr = [x,y];
			if(res[e] === undefined){
				res[e] = [cr];
			}
			else{
				res[e].push(cr);
			}
			x++;
		}
		return res;
	},
	to_arr: function(s){
		let res = new Array(8*8);
		res.fill(null);
		let k = 0;
		for(let i=0, l=s.length; i<l; ++i){
			const c = s[i];
			if(c === '/')
				continue;
			const e = this._dict.indexOf(c);
			if(e === -1){
				k += parseInt(c, 10);
				continue;
			}
			res[k] = e;
			k++;
		}
		return res;
	},
	from_arr: function(arr){
		let res = "", k = 0;
		for(let y=0; y<8; ++y){
			if(y != 0){
				res += '/';
			}
			let t = 0;
			for(let x=0; x<8; ++x){
				const c = arr[k];
				k++;
				if(c === null){
					t++;
					continue;
				}
				if(t != 0){
					res += t;
					t = 0;
				}
				res += this._dict[c];
			}
			if(t != 0){
				res += t;
			}
		}
		return res;
	}
}
