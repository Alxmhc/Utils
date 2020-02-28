package fls

import (
	"io/ioutil"
	"path/filepath"
)

type objStrings struct {
	Name   string
	Values []string
	Childs []*objStrings
}

func new_objStrings(nm string) *objStrings {
	return &objStrings{Name: nm}
}

func (obj *objStrings) AddValue(v string) {
	obj.Values = append(obj.Values, v)
}

func (obj *objStrings) AddChild(c *objStrings) {
	obj.Childs = append(obj.Childs, c)
}

func dirObj(d string, f func(string) bool, n int, obj *objStrings) {
	fls, err := ioutil.ReadDir(d)
	if err != nil {
		return
	}
	for _, o := range fls {
		name := o.Name()
		pth := filepath.Join(d, name)
		if o.IsDir() {
			if n != 0 {
				tmp := new_objStrings(name)
				dirObj(pth, f, n-1, tmp)
				if len(tmp.Values) != 0 || len(tmp.Childs) != 0 {
					obj.AddChild(tmp)
				}
			}
		} else if f(pth) {
			obj.AddValue(name)
		}
	}
}

func DirFilesObj(d string, f func(string) bool, n int) *objStrings {
	res := new_objStrings(d)
	dirObj(d, f, n, res)
	return res
}
