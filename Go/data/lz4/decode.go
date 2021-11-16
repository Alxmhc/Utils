package lz4

import (
	_data "_/data"
	"bufio"
	"encoding/binary"
)

func get_size(br *bufio.Reader, c byte) (int, error) {
	r := int(c)
	if c != 15 {
		return r, nil
	}
	for {
		b, err := br.ReadByte()
		if err != nil {
			return 0, err
		}
		r += int(b)
		if b != 255 {
			return r, nil
		}
	}
}

func Decode(br *bufio.Reader, sz int) []byte {
	res := make([]byte, 0, sz)
	for {
		b, err := br.ReadByte()
		if err != nil {
			return nil
		}
		size, err := get_size(br, b>>4)
		if err != nil {
			return nil
		}
		data := make([]byte, size)
		n, _ := br.Read(data)
		if n != size {
			return nil
		}
		res = append(res, data...)
		var offset uint16
		err = binary.Read(br, binary.LittleEndian, &offset)
		if err != nil {
			return res
		}
		len, err := get_size(br, b&0xf)
		if err != nil {
			return nil
		}
		res = _data.LZ77_repeat(uint(len+4), uint(offset), res)
		if res == nil {
			return nil
		}
	}
}
