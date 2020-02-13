package fls

import (
	"io/ioutil"
	"path/filepath"
)

func dirAll(d string, f func(string, bool) bool, n int) []string {
	var res []string
	fls, err := ioutil.ReadDir(d)
	if err != nil {
		return res
	}
	for _, o := range fls {
		pth := filepath.Join(d, o.Name())
		isDir := o.IsDir()
		if f(pth, isDir) {
			res = append(res, pth)
		}
		if isDir && n != 0 {
			tmp := dirAll(pth, f, n-1)
			res = append(res, tmp...)
		}
	}
	return res
}

func DirFiles(d string, f func(string) bool, n int) []string {
	return dirAll(d, func(pth string, isDir bool) bool { return !isDir && f(pth) }, n)
}

func DirFolders(d string, f func(string) bool, n int) []string {
	return dirAll(d, func(pth string, isDir bool) bool { return isDir && f(pth) }, n)
}
