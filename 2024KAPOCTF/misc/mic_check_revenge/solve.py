'''
from tqdm import tqdm
import crc32c

for a in tqdm(range(0x20, 0x7f)):
	for b in range(0x20, 0x7f):
		for c in range(0x20, 0x7f):
			for d in range(0x20, 0x7f):
				flag = b'pokactf2024{' + bytes([a])+bytes([b])+bytes([c])+bytes([d]) + b'}'
				if crc32c.crc32c(flag) == 0xecc9de01:
					print(flag.decode())
					exit(0)
'''
from itertools import product
from tqdm import tqdm
import hashlib

# 0x20 0x7f

values = product([bytes([i]) for i in range(0x20, 0x7f)], repeat=4)


for itervalue in tqdm(values):
    flag = b'pokactf2024{' + b''.join(itervalue) + b'}'
    sha1 = hashlib.sha1()
    sha1.update(flag)
    if sha1.hexdigest() == "7d36b2c6e1c5f4ade132f744ceb9d40e3bd5291b":
        print('\nflag : ',flag.decode())
        exit(0)
