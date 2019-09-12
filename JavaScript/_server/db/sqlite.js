//npm sqlite3
const SQL = require('./SQL.js');

module.exports = {
	select_all: function(db, t, nm, ext=''){
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
	},
	insert: function(db, t, vl, nm = null){
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
	},
	delete: function(db, t, ext){
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
}
