package main

import (
	"C"
	"_/img"
	"bytes"
	"image"
	"image/png"
	"os"
)

func Save_File_PNG(img image.Image, fout string) {
	fl, err := os.Create(fout)
	if err == nil {
		png.Encode(fl, img)
		fl.Close()
	}
}

//export SaveToGr
func SaveToGr(dat []byte, szx, szy int, fout string) {
	im := img.ReadImgGr(bytes.NewReader(dat), szx, szy)
	Save_File_PNG(im, fout)
}

//export SaveToRGB
func SaveToRGB(dat []byte, szx, szy int, fout string) {
	im := img.ReadImgRGB(bytes.NewReader(dat), szx, szy)
	Save_File_PNG(im, fout)
}

func main() {}
