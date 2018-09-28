const fs = require('fs');
const path = require('path')

//list of paths or object
function dir_fls(dir, filter, depth, need_object, onDone) {
	const res = need_object ? {'Name':dir} : [];
	_dir_fls(dir, filter, depth, res, onDone);
}

function _dir_fls(dir, filter, depth, res, onDone) {
	const need_object = !Array.isArray(res)
	if(need_object){
		res.Values = [];
		res.Childs = [];
	}
	fs.readdir(dir, (err, fls) => {
		if(err)
			return onDone(res);
		let n = fls.length;
		for(let name of fls) {
			const e = path.resolve(dir, name);
			fs.stat(e, function(err, s) {
				if (s.isDirectory()) {
					if (depth) {
						const tmp = need_object ? {'Name':name} : [];
						_dir_fls(e, filter, depth-1, tmp, rs => {
							if(need_object) {
								if(rs.Values.length || rs.Childs.length) {
									res.Childs.push(rs);
								}
							}
							else {
								res = res.concat(rs);
							}
							if(!--n)
								return onDone(res);
						})
					}
					else if(!--n)
						return onDone(res);
				}
				else {
					if(filter(name)) {
						if(need_object) {
							res.Values.push(name);
						}
						else{
							res.push(e);
						}
					}
					if(!--n)
						return onDone(res);
				}
			})
		}
	})
}

function get_range(req){
	let range = req.headers.range;
	if(!range)
		return null;
	const str = 'bytes=';
	range = range.slice(str.length);
	return range.split('-').map(e => parseInt(e, 10));
}

function calc_range(range, size){
	let start = range[0], end = range[1];
	if(isNaN(start)){
		start = end < size ? size - end : 0;
		end = size - 1;
	}
	else {
		if(isNaN(end) || end >= size){
			end = size - 1;
		}
		if(start > end)
			return null;
	}
	return [start, end];
}

function send_file(path, type, req, res){
	fs.stat(path, (err, st) => {
		if(err){
			res.writeHead(404)
			res.end()
			return
		}
		let range = get_range(req);
		if(!range){
			res.writeHead(200, {'Content-Type': type})
			fs.createReadStream(path).pipe(res);
			return
		}
		const sz = st.size;
		range = calc_range(range, sz);
		if(!range){
			res.writeHead(416, {'Content-Range': 'bytes */' + sz});
			res.end();
			return;
		}
		res.writeHead(206, { 'Content-Range': 'bytes ' + range[0] + '-' + range[1] + '/' + sz,
		                     'Content-Length': (range[1] - range[0]) + 1,
		                     'Content-Type': type });
		fs.createReadStream(path, {start: range[0], end: range[1]}).pipe(res);
	})
}

module.exports.dir_fls = dir_fls
module.exports.send = send_file
