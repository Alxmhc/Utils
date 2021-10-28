package crypt

import (
	"crypto/hmac"
	"encoding/binary"
	"hash"
)

type fhmac struct {
	hash func() hash.Hash
	H    hash.Hash
}

func (h *fhmac) Init(passw []byte) {
	h.H = hmac.New(h.hash, passw)
}

func (h *fhmac) Size() int {
	return h.H.Size()
}

func (h *fhmac) Calc0(salt, iv []byte) []byte {
	h.H.Reset()
	h.H.Write(salt)
	h.H.Write(iv)
	return h.H.Sum(nil)
}

func (h *fhmac) Calc1(b []byte) {
	h.H.Reset()
	h.H.Write(b)
	copy(b, h.H.Sum(nil))
}

type k1Func interface {
	Init(passw []byte)
	Size() int
	Calc0(salt, iv []byte) []byte
	Calc1(b []byte)
}

func pbkdf2(passw, salt []byte, iter, klen int, prf k1Func) []byte {
	prf.Init(passw)

	hLen := prf.Size()
	kl := (klen-1)/hLen + 1

	key := make([]byte, kl*hLen)
	offset := 0
	var iv [4]byte
	for b := 1; b <= kl; b++ {
		binary.BigEndian.PutUint32(iv[:], uint32(b))
		tmp := prf.Calc0(salt, iv[:])
		copy(key[offset:offset+hLen], tmp)
		for n := 1; n < iter; n++ {
			prf.Calc1(tmp)
			for i := 0; i < hLen; i++ {
				key[offset+i] ^= tmp[i]
			}
		}
		offset += hLen
	}
	return key[:klen]
}

func PBKDF2HMAC(passw, salt []byte, iter, klen int, h func() hash.Hash) []byte {
	fnc := &fhmac{hash: h}
	return pbkdf2(passw, salt, iter, klen, fnc)
}
