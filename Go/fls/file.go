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
	_, err = io.Copy(of, d)
	of.Close()
	return err
}

func WriteFileN(d io.Reader, fl string, n int64) error {
	of, err := CreateFile(fl)
	if err != nil {
		return err
	}
	_, err = io.CopyN(of, d, n)
	of.Close()
	return err
}

func WriteFileB(d []byte, fl string) error {
	of, err := CreateFile(fl)
	if err != nil {
		return err
	}
	of.Write(d)
	of.Close()
	return nil
}
