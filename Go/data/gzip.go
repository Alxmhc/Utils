package data

import (
	"bytes"
	"compress/gzip"
	"os"
)

func UngzipB(b []byte) (*gzip.Reader, error) {
	rb := bytes.NewReader(b)
	return gzip.NewReader(rb)
}

func UngzipF(f string) (*gzip.Reader, error) {
	fl, err := os.Open(f)
	if err != nil {
		return nil, err
	}
	defer fl.Close()
	return gzip.NewReader(fl)
}
