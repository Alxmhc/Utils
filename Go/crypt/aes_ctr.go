package crypt

import (
	"crypto/aes"
)

func ivIncr(iv []byte) {
	for i := len(iv) - 1; i >= 0; i-- {
		iv[i]++
		if iv[i] != 0 {
			break
		}
	}
}

func ivIncrLE(iv []byte) {
	for i := 0; i < len(iv); i++ {
		iv[i]++
		if iv[i] != 0 {
			break
		}
	}
}

func ctrDecrypt(data []byte, key []byte, ivIncr func([]byte)) {
	block, _ := aes.NewCipher(key[:])
	bs := block.BlockSize()

	iv := make([]byte, bs)
	tmp := make([]byte, bs)

	offset := 0
	for offset <= len(data)-bs {
		ivIncr(iv)
		block.Encrypt(tmp[:], iv)
		for i := 0; i < bs; i++ {
			data[offset+i] ^= tmp[i]
		}
		offset += bs
	}
	bs = len(data) - offset
	if bs != 0 {
		ivIncr(iv)
		block.Encrypt(tmp[:], iv)
		for i := 0; i < bs; i++ {
			data[offset+i] ^= tmp[i]
		}
	}
}

func AESCTRDecrypt(data, key []byte) {
	ctrDecrypt(data, key, ivIncr)
}

func AESCTRDecryptLE(data, key []byte) {
	ctrDecrypt(data, key, ivIncrLE)
}
