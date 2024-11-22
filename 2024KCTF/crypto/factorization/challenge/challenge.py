from Crypto.Util.number import bytes_to_long, getPrime, inverse
from secret import FLAG # FLAG is here!
import random

def gen_param() -> (int, int, int, int, int, int):
    p, q = getPrime(512), getPrime(512)
    n = p * q
    a, b = random.getrandbits(64), random.getrandbits(64)

    w = (p + a)**2 + (q + b)**2

    return p, q, a, b, n, w

def encrypt(m, n) -> int:
    e = 0x10001
    c = pow(m, e, n)
    return c

def decrypt(c, p, q, n) -> int:
    e = 0x10001
    phi = (p - 1) * (q - 1)
    d = inverse(e, phi)
    m = pow(c, d, n)
    return m

p, q, a, b, n, w = gen_param()
m = bytes_to_long(FLAG)
c = encrypt(m, n)

print(f'a = {hex(a)}')
print(f'b = {hex(b)}')
print(f'n = {hex(n)}')
print(f'w = {hex(w)}')

print(f'e = {hex(0x10001)}')
print(f'c = {hex(c)}')