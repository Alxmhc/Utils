import (
	"io/ioutil"
)

func dir_(d string, f func(string) bool, n int, is_dir bool) []string {
	var res []string
	fls,err := ioutil.ReadDir(d)
	if err != nil {
		return res
	}
	for _,o := range fls {
		pth := d + "/" + o.Name()
		if o.IsDir() {
			if is_dir && f(pth) {
				res = append(res, pth)
			}
			if n != 0 {
				tmp := dir_(pth, f, n-1, is_dir)
				res = append(res, tmp...)
			}
		} else if !is_dir && f(pth) {
			res = append(res, pth)
		}
	}
	return res
}

func dir_files(d string, f func(string) bool, n int) []string {
	return dir_(d, f, n, false)
}

func dir_folders(d string, f func(string) bool, n int) []string {
	return dir_(d, f, n, true)
}
