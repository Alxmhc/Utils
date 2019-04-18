package fls

import (
	"io"
	"os"
	"path/filepath"
)

func CreateFile(pth string) (*os.File, error) {
	os.MkdirAll(filepath.Dir(pth), os.ModePerm)
	return os.Create(pth)
}

func WriteFile(d io.Reader, fl string) error {
	of, err := CreateFile(fl)
	if err != nil {
		return err
	}
	defer of.Close()
	_, err = io.Copy(of, d)
	return err
}

func WriteFileB(d []byte, fl string) error {
	of, err := CreateFile(fl)
	if err != nil {
		return err
	}
	defer of.Close()
	of.Write(d)
	return nil
}
