Chess = {}

Chess.color = ['white', 'black'];
Chess.type = ['pawn', 'rook', 'knight', 'bishop', 'queen', 'king'];

Chess.inf = function(p){
	if(p === null)
		return null;
	if(p < 6)
		return {color: 0, type: p};
	return {color: 1, type: p - 6};
}

Chess.fen = 'PRNBQKprnbqk'

Chess.fen_to_pos = function(s){
	let res = {}, x=0, y=0;
	for(let i=0, l=s.length; i<l; ++i){
		const c = s[i];
		if(c === '/'){
			y++;
			x = 0;
			continue;
		}
		const e = Chess.fen.indexOf(c);
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
}

Chess.pos_to_obj = function(p){
	return vn2.from_pos(p, [8,8]);
}

Chess.obj_to_fen = function(o){
	let res = "";
	for(let y=0; y<8; ++y){
		if(y != 0){
			res += '/';
		}
		let t = 0;
		const cx = o.getx(y);
		for(c of cx){
			if(c === null){
				t++;
				continue;
			}
			if(t != 0){
				res += t;
				t = 0;
			}
			res += Chess.fen[c];
		}
		if(t != 0){
			res += t;
		}
	}
	return res;
}

Chess.pos_to_fen = function(p){
	const o = Chess.pos_to_obj(p);
	return Chess.obj_to_fen(o);
}
