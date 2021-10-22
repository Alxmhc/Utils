package net_

import "encoding/binary"

const (
	E_IPv4 uint16 = 0x0800
	E_IPv6 uint16 = 0x86dd
	E_ARP  uint16 = 0x0806

	E_TCP    byte = 6
	E_UDP    byte = 17
	E_IGMP   byte = 2
	E_ICMPv6 byte = 58
)

type Ethernet_II struct {
	MACsrc [6]byte
	MACdst [6]byte
	Type   uint16
}

func (p *Ethernet_II) Read(data []byte) byte {
	if len(data) < 14 {
		return 0
	}
	copy(p.MACsrc[:], data[6:12])
	copy(p.MACdst[:], data[:6])
	p.Type = binary.BigEndian.Uint16(data[12:14])
	return 14
}

type IPv4 struct {
	IPsrc [4]byte
	IPdst [4]byte
	ID    uint16
	Type  byte
}

func (p *IPv4) Read(data []byte) byte {
	if len(data) < 20 {
		return 0
	}
	sz := (data[0] & 0x0f) << 2
	if len(data) < int(sz) || sz < 20 {
		return 0
	}
	copy(p.IPsrc[:], data[12:16])
	copy(p.IPdst[:], data[16:20])
	p.ID = binary.BigEndian.Uint16(data[4:6])
	p.Type = data[9]
	return sz
}

type IPv6 struct {
	IPsrc [16]byte
	IPdst [16]byte
	Type  byte
}

func (p *IPv6) Read(data []byte) byte {
	if len(data) < 40 {
		return 0
	}
	copy(p.IPsrc[:], data[8:24])
	copy(p.IPdst[:], data[24:40])
	p.Type = data[6]
	return 40
}

type TCP struct {
	PortSrc uint16
	PortDst uint16
	Num     uint32
	NumAck  uint32
	f       byte
}

func (p *TCP) Read(data []byte) byte {
	if len(data) < 13 {
		return 0
	}
	sz := (data[12] >> 4) << 2
	if len(data) < int(sz) || sz < 20 {
		return 0
	}
	p.PortSrc = binary.BigEndian.Uint16(data[0:2])
	p.PortDst = binary.BigEndian.Uint16(data[2:4])
	p.Num = binary.BigEndian.Uint32(data[4:8])
	p.NumAck = binary.BigEndian.Uint32(data[8:12])
	p.f = data[13]
	return sz
}

func (p *TCP) ACK() bool {
	return p.f&0x10 != 0
}
func (p *TCP) PSH() bool {
	return p.f&0x08 != 0
}
func (p *TCP) RST() bool {
	return p.f&0x04 != 0
}
func (p *TCP) SYN() bool {
	return p.f&0x02 != 0
}
func (p *TCP) FIN() bool {
	return p.f&0x01 != 0
}

type UDP struct {
	PortSrc uint16
	PortDst uint16
}

func (p *UDP) Read(data []byte) byte {
	if len(data) < 8 {
		return 0
	}
	p.PortSrc = binary.BigEndian.Uint16(data[0:2])
	p.PortDst = binary.BigEndian.Uint16(data[2:4])
	return 8
}
