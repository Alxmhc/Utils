import (
	"math"
)

type v3 struct {
	x,y,z float64
}

func (v *v3) dot(a *v3) float64 {
	return v.x*a.x + v.y*a.y + v.z*a.z
}

func (v *v3) abs() float64 {
	return math.Sqrt( v.dot(v) )
}
