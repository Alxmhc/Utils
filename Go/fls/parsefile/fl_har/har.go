package fl_har

import (
	"bytes"
	"encoding/base64"
	"encoding/json"
	"io/ioutil"
	"net/http"
	"os"
	"strconv"
	"strings"
)

type header struct {
	Name  string
	Value string
}

type request struct {
	Method   string
	Url      string
	Headers  []header
	PostData struct {
		Text string
	}
}

type response struct {
	Status     int
	StatusText string
	Headers    []header
	Content    struct {
		Text     string
		Encoding string
	}
}

type entrie struct {
	Request  request
	Response response
}

func (e entrie) getReq() *http.Request {
	rq := e.Request
	req, _ := http.NewRequest(rq.Method, rq.Url, strings.NewReader(rq.PostData.Text))
	for _, hdr := range rq.Headers {
		name := hdr.Name
		if name[0] == ':' {
			continue
		}
		req.Header.Set(strings.Title(name), hdr.Value)
	}
	return req
}

func (e entrie) getResp() *http.Response {
	rs := e.Response
	if rs.Status == 0 {
		return nil
	}
	res := &http.Response{Proto: "HTTP/1.1", ProtoMajor: 1, ProtoMinor: 1, Header: make(http.Header)}
	res.StatusCode = rs.Status
	res.Status = rs.StatusText

	body := rs.Content.Text
	if body != "" {
		var data []byte
		if rs.Content.Encoding == "base64" {
			data, _ = base64.StdEncoding.DecodeString(body)
		} else {
			data = []byte(body)
		}
		res.ContentLength = int64(len(data))
		res.Body = ioutil.NopCloser(bytes.NewReader(data))
	}

	for _, hdr := range rs.Headers {
		name := strings.ToLower(hdr.Name)
		if name == "status" ||
			name == "content-encoding" ||
			name == "transfer-encoding" {
			continue
		}
		res.Header.Set(strings.Title(name), hdr.Value)
	}
	res.Header.Set("Content-Length", strconv.FormatInt(res.ContentLength, 10))
	return res
}

type har struct {
	Log struct {
		Entries []entrie
	}
}

func Parse(fname string) []http.Request {
	fl, err := os.Open(fname)
	if err != nil {
		return nil
	}
	var data har
	err = json.NewDecoder(fl).Decode(&data)
	if err != nil {
		return nil
	}
	var res []http.Request
	for _, entrie := range data.Log.Entries {
		req := entrie.getReq()
		req.Response = entrie.getResp()
		res = append(res, *req)
	}
	return res
}
