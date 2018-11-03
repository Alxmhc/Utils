//npm sqlite3
const SQL = require('./SQL.js');

const sqlite = {}
sqlite.select_all = function(db, t, nm, ext=''){
	const q = SQL.select(t, nm, ext);
	return new Promise((res, rej) => {
		db.all(q, function(err, rs){
			if(err){
				rej(err);
				return;
			}
			res(rs);
		})
	})
}
sqlite.insert = function(db, t, vl, nm = null){
	const q = SQL.insert(t, vl, nm);
	return new Promise((res, rej) => {
		db.run(q, function(err){
			if(err){
				rej(err);
				return;
			}
			res(this.lastID);
		})
	})
}
sqlite.delete = function(db, t, ext){
	const q = SQL.delete(t, ext);
	return new Promise((res, rej) => {
		db.run(q, function(err){
			if(err){
				rej(err);
				return;
			}
			res();
		})
	})	
}

module.exports = sqlite
