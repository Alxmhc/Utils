package fls

import (
	"os"
	"path/filepath"
)

func DirAll(d string, fltr func(os.DirEntry) bool) []string {
	var res []string
	pths := []string{d}
	for {
		sz := len(pths)
		if sz == 0 {
			break
		}
		pth := pths[0]
		fls, err := os.ReadDir(pth)
		pths = pths[1:]
		if err != nil {
			continue
		}
		for _, fl := range fls {
			fpth := filepath.Join(pth, fl.Name())
			if fltr(fl) {
				res = append(res, fpth)
			}
			if fl.IsDir() {
				pths = append(pths, fpth)
			}
		}
	}
	return res
}
