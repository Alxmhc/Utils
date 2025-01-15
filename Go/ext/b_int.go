package main

import (
	"C"
	"math/big"
)

//export Mod
func Mod(a, m []byte) int {
	a1 := new(big.Int)
	a1.SetBytes(a)
	m1 := new(big.Int)
	m1.SetBytes(m)
	r1 := new(big.Int)
	r1.Mod(a1, m1)
	r := r1.Bytes()
	copy(m, r)
	return len(r)
}

func main() {}
