function pos2_to_arr(p, sz){
	const sx = sz[0], sy = sz[1];
	let res = new Array(sx*sy);
	res.fill(null);
	for(let k in p){
		for(let cr of p[k]){
			res[cr[0]+cr[1]*sy] = k;
		}
	}
	return res;
}

function arr_to_pos2(arr, sz){
	let res = {}, k = 0;
	for(let y = 0; y < sz[1]; ++y){
		for(let x = 0; x < sz[0]; ++x){
			const c = this.arr[k];
			k++;
			if(c === null)
				continue;
			const cr = [x,y];
			if(res[c] === undefined){
				res[c] = [cr];
			}
			else{
				res[c].push(cr);
			}
		}
	}
	return res;
}
