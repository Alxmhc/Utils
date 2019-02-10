Chess = {}
Chess.fen = 'PRNBQKprnbqk'

Chess.pos_from_fen = function(s){
	const res = new Uint8Array(64);
	res.fill(0);
	for(let j=0, l=s.length, i=0; i<l; ++i){
		const c = s[i];
		if(c === '/')
			continue;
		const e = Chess.fen.indexOf(c) + 1;
		if(e == 0){
			j += parseInt(c, 10);
			continue;
		}
		res[j] = e;
		j++;
	}
	return res;
}

Chess.pos_to_fen = function(pos){
	let res = "", t = 0;
	for(let l=pos.length, i=0; i<l; ++i){
		if(i%8 == 0 && i != 0){
			if(t != 0){
				res += t;
				t = 0;
			}
			res += '/';
		}
		if(pos[i] == 0){
			t++;
			continue;
		}
		if(t != 0){
			res += t;
			t = 0;
		}
		res += Chess.fen[ pos[i] - 1 ];
	}
	return res;
}
