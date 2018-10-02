import (
	"strings"
)

func list_dict(st []string, d string) map[string]string {
	r := make(map[string]string)
	n := len(d)
	for _,s := range st {
		p := strings.Index(s, d)
		if p != -1 {
			r[s[:p]] = s[p+n:]
		}
	}
	return r
}
