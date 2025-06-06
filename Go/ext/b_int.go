package main

import (
	"C"
	"math/big"
)

//export ModInv
func ModInv(a, m []byte) int {
	a1 := new(big.Int)
	a1.SetBytes(a)
	m1 := new(big.Int)
	m1.SetBytes(m)

	r1 := new(big.Int)
	r1.ModInverse(a1, m1)
	r := r1.Bytes()
	copy(m, r)
	return len(r)
}

//export PowMod
func PowMod(a, b, m []byte) int {
	a1 := new(big.Int)
	a1.SetBytes(a)
	b1 := new(big.Int)
	b1.SetBytes(b)
	m1 := new(big.Int)
	m1.SetBytes(m)

	r1 := new(big.Int)
	r1.Exp(a1, b1, m1)
	r := r1.Bytes()
	copy(m, r)
	return len(r)
}

func main() {}
