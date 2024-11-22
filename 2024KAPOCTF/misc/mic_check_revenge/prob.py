import crc32c
flag = open('./flag.txt', 'rb').read()
print(hex(crc32c.crc32c(flag)))
