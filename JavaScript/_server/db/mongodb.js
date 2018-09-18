//npm mongodb
const util = require('util')

const mongodb = {}
mongodb.tbl_list = function(conn){
	return util.promisify(conn.collections).bind(conn);
}
mongodb.select_all = function(collection, query){
	return collection.find(query).toArray()
}

module.exports = mongodb
