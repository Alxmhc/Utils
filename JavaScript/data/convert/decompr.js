function LZ77_repeat(sz, dist, out){
	if(dist == 0 || out.length < dist)
		return null;
	if(sz == 0)
		return out;
	const beg = out.length - dist;
	while(sz > dist){
		out = out.concat( out.slice(beg, beg + dist) );
		sz -= dist;
		dist += dist;
	}
	return out.concat( out.slice(beg, beg + sz) );	
}
