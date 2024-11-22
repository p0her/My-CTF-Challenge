from pwn import *
import random

REMOTE = True
if REMOTE:
    HOST, PORT = 'host3.dreamhack.games', 18335
else:
    HOST, PORT = 'localhost', 12345

io = remote(HOST, PORT)
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

io.interactive()
