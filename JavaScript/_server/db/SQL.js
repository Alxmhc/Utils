module.exports = {
	insert: function(t, vl, nm = null){
		let s = 'INSERT INTO '+t;
		if(nm){
			s += '('+nm.join(',')+')';
		}
		return s+' VALUES('+vl.map(e=>typeof e!='string'?e:'"'+e+'"').join(',')+')';
	},
	select: function(t, nm, ext = ''){
		return 'SELECT '+nm.join(',')+' FROM '+t+ext;
	},
	delete: function(t, ext){
		return 'DELETE FROM '+t+' WHERE '+ext;
	}
}
