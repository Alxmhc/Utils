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
	db.run(q, function(err){
		return err ? 0 : this.lastID;
	})
}

module.exports = sqlite
