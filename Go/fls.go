import (
	"io/ioutil"
)

func dir_fls(d string, f func(string) bool, n int) []string {
	var res []string
	fls,err := ioutil.ReadDir(d)
	if err != nil {
		return res
	}
	for _,fl := range fls {
		if fl.IsDir() {
			if n != 0 {
				tmp := dir_fls(d + "/" + fl.Name(), f, n-1)
				res = append(res, tmp...)
			}
		} else if f(fl.Name()) {
			res = append(res, d + "/" + fl.Name())
		}
	}
	return res
}
