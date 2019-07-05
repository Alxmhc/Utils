Chess = {}

Chess.color = {
	none: 0,
	white: 1,
	black: 2
}
Chess.type = {
	none: 0,
	pawn: 1,
	rook: 2,
	knight: 3,
	bishop: 4,
	queen: 5,
	king: 6
}
Chess.inf = function(p){
	if(p === 0)
		return {color: 0, type: 0};
	if(p < 7)
		return {color: 1, type: p};
	return {color: 2, type: p - 6};
}

Chess.fen = 'PRNBQKprnbqk'

Chess.fen_to_moves = function(s){
	let res = [], x=0, y=0;
	for(let i=0, l=s.length; i<l; ++i){
		const c = s[i];
		if(c === '/'){
			y++;
			x = 0;
			continue;
		}
		const e = Chess.fen.indexOf(c) + 1;
		if(e == 0){
			x += parseInt(c, 10);
			continue;
		}
		res.push([[x,y],e]);
		x++;
	}
	return res;
}

Chess.pos_to_fen = function(v){
	let res = "", t = 0;
	for(let y=0; y<8; ++y){
		if(y != 0){
			if(t != 0){
				res += t;
				t = 0;
			}
			res += '/';
		}
		for(let x=0; x<8; ++x){
			const o = v.get([x,y]);
			if(o == 0){
				t++;
				continue;
			}
			if(t != 0){
				res += t;
				t = 0;
			}
			res += Chess.fen[o-1];
		}
	}
	return res;
}
