const fs = require('fs');
const path = require('path')

function dir_fls(dir, fn, depth, onDone) {
	const res = {};
	res.Name = dir;
	_dir_fls(dir, fn, depth, res, onDone);
}

function _dir_fls(dir, fn, depth, res, onDone) {
	res.Values = [];
	res.Childs = [];
	fs.readdir(dir, (err, fls) => {
		if(err)
			return onDone(res)
		let n = fls.length;
		for(let name of fls) {
			const e = path.resolve(dir, name);
			fs.stat(e, function(err, s) {
				if (s.isDirectory()) {
					if (depth) {
						const tmp = {};
						tmp.Name = name;
						_dir_fls(e, fn, depth-1, tmp, rs => {
							if(rs.Values.length || rs.Childs.length) {
								res.Childs.push(rs);
							}
							if(!--n)
								return onDone(res);
						})
					}
					else if(!--n)
						return onDone(res);
				}
				else {
					if(fn(name)) {
						res.Values.push(name);
					}
					if(!--n)
						return onDone(res);
				}
			})
		}
	})
}

function send_file(path, type, req, res){
	fs.stat(path, (err, st) => {
		if(err){
			res.writeHead(404)
			res.end()
			return
		}
		const range = req.headers.range;
		if(!range){
			res.writeHead(200, {'Content-Type': type})
			fs.createReadStream(path).pipe(res);
			return
		}
		const sz = st.size
		const str = 'bytes='
		const arr = range.slice(str.length).split('-')
		let start, end
		if(arr[0]){
			start = parseInt(arr[0], 10)
			end = arr[1] ? parseInt(arr[1], 10) : 0
			if(!end || end>=sz)
				end = sz - 1
		}
		else{
			start = sz - parseInt(arr[1], 10)
			end = sz - 1
		}
		res.writeHead(206, { 'Content-Range': 'bytes ' + start + '-' + end + '/' + sz,
		                     'Content-Length': (end - start) + 1,
		                     'Content-Type': type });
		fs.createReadStream(path, {start: start, end: end}).pipe(res);
	})
}

module.exports.dir_fls = dir_fls
module.exports.send = send_file
