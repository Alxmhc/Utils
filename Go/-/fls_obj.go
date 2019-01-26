import (
	"io/ioutil"
)

func dir_fls(d string, f func(string) bool, n int) *obj_strings {
	res := new_obj_strings(d)
	_dir_fls(d, f, n, res)
	return res
}

func _dir_fls(d string, f func(string) bool, n int, obj *obj_strings) {
	fls,err := ioutil.ReadDir(d)
	if err != nil {
		return
	}
	for _,fl := range fls {
		name := fl.Name()
		if fl.IsDir() {
			if n != 0 {
				tmp := new_obj_strings(name)
				_dir_fls(d + "/" + name, f, n-1, tmp)
				if(len(tmp.Values) != 0 || len(tmp.Childs) != 0) {
					obj.add_child(tmp)
				}
			}
		} else if f(name) {
			obj.add_value(name)
		}
	}
}
