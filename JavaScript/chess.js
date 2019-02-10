Chess = {}
Chess.fen = 'PRNBQKprnbqk'

Chess.from_fen = function(s){
	const res = new Uint8Array(64)
	res.fill(0)
	for(let j=0, l=s.length, i=0; i<l; ++i){
		const c = s[i]
		if(c === '/')
			continue
		const e = Chess.fen.indexOf(c) + 1
		if(e == 0){
			j += parseInt(c, 10)
			continue
		}
		res[j] = e
		j++
	}
	return res
}
