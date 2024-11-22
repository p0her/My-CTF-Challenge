from pwn import *
from Crypto.Util.number import *
from randcrack import RandCrack

rc = RandCrack()

HOST = '3.34.91.236'
PORT = 20002

io = remote(HOST, PORT)

p = int(io.recvline())
A = int(io.recvline())
B = int(io.recvline())

io.success(f'p : {hex(p)}')
for i in range(624):
    t = int(io.recvline())
    x = ((t - B) * inverse(A, p)) % p
    rc.submit(x)

flag = ''

while True:
    try:
        enc = int(io.recvline())
        A = rc.predict_getrandbits(32)
        B = rc.predict_getrandbits(32)
        C = rc.predict_getrandbits(32)
        enc -= C

        for x in range(0x20, 0x7f):
            if A*x**2 + B*x == enc:
                flag += chr(x)
                break
    except:
        break

io.success(flag)