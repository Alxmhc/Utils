let SQL = {}
SQL.insert = function(t, vl, nm = null){
	let s = 'INSERT INTO '+t;
	if(nm){
		s += '('+nm.join(',')+')';
	}
	return s+' VALUES('+vl.map(e=>typeof e!='string'?e:'"'+e+'"').join(',')+')';
}
SQL.select = function(t, nm, ext = ''){
	return 'SELECT '+nm.join(',')+' FROM '+t+ext;
}

module.exports = SQL
