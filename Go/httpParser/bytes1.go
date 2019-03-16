package httpParser

import (
	"bytes"
	"strings"
)

func parseBytes1(frame []byte) ([3]string, []string, []byte) {
	p := bytes.Index(frame, []byte("\r\n\r\n"))
	body := frame[p+4:]
	str := string(frame[:p])
	p = strings.Index(str, "\r\n")
	var ln [3]string
	copy(ln[:], strings.SplitN(str[:p], " ", 3))
	return ln, strings.Split(str[p+2:], "\r\n"), body
}

func FromBytes1Req(frame []byte) *http_req {
	ln, hdr, body := parseBytes1(frame)
	dict := make(map[string]string)
	for _, s := range hdr {
		p := strings.Index(s, ": ")
		dict[strings.ToLower(s[:p])] = s[p+2:]
	}
	return &http_req{Method: ln[0], URL: ln[1], Ver: ln[2], Hdr: dict, Body: body}
}

func FromBytes1Resp(frame []byte) *http_resp {
	ln, hdr, body := parseBytes1(frame)
	dict := make(map[string][]string)
	for _, s := range hdr {
		p := strings.Index(s, ": ")
		k := strings.ToLower(s[:p])
		arr, _ := dict[k]
		dict[k] = append(arr, s[p+2:])
	}
	return &http_resp{Ver: ln[0], Code: ln[1], Text: ln[2], Hdr: dict, Body: body}
}
