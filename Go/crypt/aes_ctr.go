package crypt

import (
	"crypto/aes"
)

type IV interface {
	Data() []byte
	Incr()
}

func AESctrEncrypt(data []byte, key []byte, v IV) {
	block, _ := aes.NewCipher(key[:])
	bs := block.BlockSize()

	tmp := make([]byte, bs)
	offset := 0
	for offset <= len(data)-bs {
		v.Incr()
		block.Encrypt(tmp[:], v.Data())
		for i := 0; i < bs; i++ {
			data[offset+i] ^= tmp[i]
		}
		offset += bs
	}
	bs = len(data) - offset
	if bs != 0 {
		v.Incr()
		block.Encrypt(tmp[:], v.Data())
		for i := 0; i < bs; i++ {
			data[offset+i] ^= tmp[i]
		}
	}
}

func AESctrDecrypt(data []byte, key []byte, v IV) {
	AESctrEncrypt(data, key, v)
}
