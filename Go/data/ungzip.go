import (
	"bytes"
	"compress/gzip"
	"io/ioutil"
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
