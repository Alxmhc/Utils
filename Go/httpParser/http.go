package httpParser

type http_req struct {
	Method string
	URL    string
	Ver    string
	Hdr    map[string]string
	Body   []byte
}

type http_resp struct {
	Ver  string
	Code string
	Text string
	Hdr  map[string][]string
	Body []byte
}
