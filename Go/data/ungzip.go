package data

import (
	"bytes"
	"compress/gzip"
	"io"
	"io/ioutil"
	"os"
)

func ungzip(r io.Reader) ([]byte, error) {
	gz,err := gzip.NewReader(r)
	if err != nil {
		return nil, err
	}
	defer gz.Close()
	b,err := ioutil.ReadAll(gz)
	return b, err	
}

func UngzipBB(b []byte) ([]byte, error) {
	rb := bytes.NewReader(b)
	return ungzip(rb)
}

func UngzipFB(f string) ([]byte, error) {
	fl,err := os.Open(f)
	if err != nil {
		return nil, err
	}
	defer fl.Close()
	return ungzip(fl)
}
