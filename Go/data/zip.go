package data

import (
	"Go/fls"
	"archive/zip"
	"os"
	"path/filepath"
	"strings"
)

func Unzip(fl string, pth string) {
	r, err := zip.OpenReader(fl)
	if err != nil {
		return
	}
	defer r.Close()
	pth = filepath.Clean(pth)
	for _, f := range r.File {
		fo := filepath.Join(pth, f.Name)
		if !strings.HasPrefix(fo, pth) {
			continue
		}
		if f.FileInfo().IsDir() {
			os.MkdirAll(fo, f.Mode())
			continue
		}
		zfl, err := f.Open()
		if err != nil {
			continue
		}
		fls.WriteFile(zfl, fo)
		zfl.Close()
	}
}
