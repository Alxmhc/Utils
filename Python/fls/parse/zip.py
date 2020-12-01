comprType = {"NO":0, "Deflate":8, "Deflate64":9, "BZIP2":12, "LZMA":14, "PPMd":98}
encrType = {"NO":0, "UNKNOWN":1, "AES128":2, "AES192":3, "AES256":4}

def read_inf(fl):
	while True:
		h = fl.read(4)
		if h == b"\x50\x4b\x03\x04":
			res = {}
			hdr = fl.read(26)
			if hdr == b'':
				break
			res["encryption"] = hdr[2] & 1
			res["method"] = int.from_bytes(hdr[4:6], byteorder='little')
			res["crc32"] = hdr[10:14]
			res["fsize"] = int.from_bytes(hdr[18:22], byteorder='little')
			res["psize"] = int.from_bytes(hdr[14:18], byteorder='little')

			szfn = int.from_bytes(hdr[22:24], byteorder='little')
			res["fname"] = fl.read(szfn).decode("utf-8")
			szex = int.from_bytes(hdr[24:26], byteorder='little')
			if szex != 0:
				ext = fl.read(szex);
				if res["method"] == 99:
					res["encryption"] = ext[8] + 1;
					res["method"] = int.from_bytes(ext[9:11], byteorder='little')
			res["ppos"] = fl.tell()
			fl.seek(res["psize"], 1)
			yield res
		else:
			break
