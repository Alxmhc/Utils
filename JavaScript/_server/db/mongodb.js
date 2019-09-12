//npm mongodb
const util = require('util');

module.exports = {
	tbl_list: function(conn){
		return util.promisify(conn.collections).bind(conn);
	},
	select_all: function(collection, query){
		return collection.find(query).toArray()
	}
}
