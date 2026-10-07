package main

import (
	"C"
	"_/img"
	"bytes"
)

//export SaveToGr
func SaveToGr(dat []byte, szx, szy int, fout string) {
	im := img.ReadImgGr(bytes.NewReader(dat), szx, szy)
	img.Save_File_PNG(im, fout)
}

//export SaveToRGB
func SaveToRGB(dat []byte, szx, szy int, fout string) {
	im := img.ReadImgRGB(bytes.NewReader(dat), szx, szy)
	img.Save_File_PNG(im, fout)
}

func main() {}
