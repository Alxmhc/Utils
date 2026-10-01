package fls

import (
	"os"
	"path/filepath"
)

func DirAll(d string, fltr func(os.DirEntry) bool) []string {
	var res []string
	pths := []string{d}
	for len(pths) != 0 {
		pth := pths[0]
		pths = pths[1:]
		fls, err := os.ReadDir(pth)
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
