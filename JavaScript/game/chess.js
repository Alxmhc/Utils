Chess = {
	sz: [8,8],
	color: ['white', 'black'],
	type: ['pawn', 'rook', 'knight', 'bishop', 'queen', 'king']
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
		let res = new Array(Chess.sz[0]*Chess.sz[1]);
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
		const sx = Chess.sz[0], sy = Chess.sz[1];
		let res = "", k = 0;
		for(let y=0; y<sy; ++y){
			if(y != 0){
				res += '/';
			}
			let t = 0;
			for(let x=0; x<sx; ++x){
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
