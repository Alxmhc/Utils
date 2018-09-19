import (
	"bytes"
	"compress/gzip"
	"io/ioutil"
	"os"
)

func ungzip_bb(b []byte) ([]byte, error) {
	rb := bytes.NewReader(b)
	gz,err := gzip.NewReader(rb)
	if err != nil {
		return make([]byte,0), err
	}
	defer gz.Close()
	r,_ := ioutil.ReadAll(gz)
	return r, nil
}

func ungzip_bf(b []byte, fname string) {
	r,err := ungzip_bb(b)
	if err != nil {
		return
	}
	file,_ :=os.Create(fname)
	defer file.Close()
	file.Write(r)
}
