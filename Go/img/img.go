package img

import (
	"image"
	"image/color"
	"io"
)

func ReadImgGr(dat io.Reader, szx, szy int) image.Image {
	img := image.NewGray(image.Rect(0, 0, szx, szy))
	c := make([]byte, szx)
	for y := 0; y < szy; y++ {
		n, _ := dat.Read(c)
		if n != szx {
			return nil
		}	
		for x := 0; x < szx; x++ {
			img.Set(x, y, color.Gray{c[x]})
		}
	}
	return img
}

func ReadImgRGB(dat io.Reader, szx, szy int) image.Image {
	img := image.NewRGBA(image.Rect(0, 0, szx, szy))
	c := make([]byte, 3)
	for y := 0; y < szy; y++ {
		for x := 0; x < szx; x++ {
			n, _ := dat.Read(c)
			if n != 3 {
				return nil
			}
			img.SetRGBA(x, y, color.RGBA{c[0], c[1], c[2], 0xff})
		}
	}
	return img
}
