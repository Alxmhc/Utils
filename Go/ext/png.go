package main

import (
	"C"
	"_/img"
	"bytes"
	"image/png"
	"os"
)

//export SaveToGr
func SaveToGr(dat []byte, szx, szy int, fout string) {
	fl, err := os.Create(fout)
	if err != nil {
		return
	}
	defer fl.Close()
	im := img.ReadImgGr(bytes.NewReader(dat), szx, szy)
	png.Encode(fl, im)
}

//export SaveToRGB
func SaveToRGB(dat []byte, szx, szy int, fout string) {
	fl, err := os.Create(fout)
	if err != nil {
		return
	}
	defer fl.Close()
	im := img.ReadImgRGB(bytes.NewReader(dat), szx, szy)
	png.Encode(fl, im)
}

func main() {
}
