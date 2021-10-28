package img

import (
	"image"
	"image/color"
	"io"
)

func ReadImgRGB(dat io.Reader, szx, szy int) image.Image {
	im := image.NewRGBA(image.Rectangle{image.Point{0, 0}, image.Point{szx, szy}})
	c := make([]byte, 3)
	for y := 0; y < szy; y++ {
		for x := 0; x < szx; x++ {
			n, _ := dat.Read(c)
			if n != 3 {
				return nil
			}
			im.SetRGBA(x, y, color.RGBA{c[0], c[1], c[2], 0xff})
		}
	}
	return im
}
