package crypt

import (
	"crypto/cipher"
)

type IV interface {
	Init()
	Data() []byte
	Incr()
}

func CR_CTR_Encrypt(cr cipher.Block, v IV, data []byte) {
	v.Init()
	bs := cr.BlockSize()
	tmp := make([]byte, bs)
	offset := 0
	for offset <= len(data)-bs {
		cr.Encrypt(tmp[:], v.Data())
		v.Incr()
		for i := 0; i < bs; i++ {
			data[offset+i] ^= tmp[i]
		}
		offset += bs
	}
	bs = len(data) - offset
	if bs != 0 {
		cr.Encrypt(tmp[:], v.Data())
		v.Incr()
		for i := 0; i < bs; i++ {
			data[offset+i] ^= tmp[i]
		}
	}
}

func CR_CTR_Decrypt(cr cipher.Block, v IV, data []byte) {
	CR_CTR_Encrypt(cr, v, data)
}
