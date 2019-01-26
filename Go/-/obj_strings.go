type obj_strings struct {
	Name string
	Values []string
	Childs []*obj_strings
}

func new_obj_strings(nm string) *obj_strings {
	return &obj_strings{Name: nm}
}

func (obj *obj_strings) add_value(v string) {
	obj.Values = append(obj.Values, v)
}

func (obj *obj_strings) add_child(c *obj_strings) {
	obj.Childs = append(obj.Childs, c)
}
