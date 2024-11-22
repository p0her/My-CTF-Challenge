'''
if version == 1 and isinstance(a, (str, bytes)): // random.seed(?, version=1)
            a = a.decode('latin-1') if isinstance(a, bytes) else a
            x = ord(a[0]) << 7 if a else 0
            for c in map(ord, a):
                x = ((1000003 * x) ^ c) & 0xFFFFFFFFFFFFFFFF
            x ^= len(a)
            a = -2 if x == -1 else x

elif version == 2 and isinstance(a, (str, bytes, bytearray)): // random.seed(?, version=2)
    global _sha512
    if _sha512 is None:
        try:
            # hashlib is pretty heavy to load, try lean internal
            # module first
            from _sha2 import sha512 as _sha512
        except ImportError:
            # fallback to official implementation
            from hashlib import sha512 as _sha512

    if isinstance(a, str):
        a = a.encode()
    a = int.from_bytes(a + _sha512(a).digest()) << random.seed(integer) == random.seed(some_string != integer)
'''

import hashlib
from pwn import *

REMOTE = True
if REMOTE:
    HOST, PORT = '54.180.153.92', 30000
else:
    HOST, PORT = 'localhost', 30000

io = remote(HOST, PORT)

def stage1():
    import random
    MAGIC = 'p0her'
    x = ord(MAGIC[0]) << 7 if MAGIC else 0
    for c in map(ord, MAGIC):
        x = ((1000003 * x) ^ c) & 0xFFFFFFFFFFFFFFFF
    x ^= len(MAGIC)
    a = -2 if x == -1 else x

    x = a
    y = MAGIC

    random.seed(x, version=1)
    state1 = random.getrandbits(0x10001)
    random.seed(y, version=1)
    state2 = random.getrandbits(0x10001)

    assert state1 == state2

    io.sendlineafter('x > ', str(x))
    io.sendlineafter('y > ', str(y))

def stage2():
    import random
    MAGIC = b'p0her'
    sha512 = hashlib.sha512()
    sha512.update(MAGIC)

    hack = MAGIC + sha512.digest()
    x = int.from_bytes(hack, byteorder='big')
    y = MAGIC

    random.seed(x, version=2)
    state1 = random.getrandbits(0x10001)
    random.seed(y, version=2)
    state2 = random.getrandbits(0x10001)

    assert state1 == state2
    print(x, y)
    io.sendlineafter('x > ', str(x))
    io.sendlineafter('y > ', y)

stage1()
stage2()
io.interactive()
