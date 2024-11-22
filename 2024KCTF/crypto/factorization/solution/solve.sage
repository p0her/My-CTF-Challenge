from itertools import combinations
from Crypto.Util.number import inverse, long_to_bytes
from pwn import *
REMOTE = True
if REMOTE:
    HOST, PORT = '54.180.153.92', int(20002)
else:
    HOST, PORT = 'localhost', int(20002)

io = remote(HOST, PORT)
context.log_level = 'debug'

def decrypt(c, p, q, n) -> int:
    e = 0x10001
    phi = (p - 1) * (q - 1)
    d = inverse_mod(e, phi)
    m = power_mod(c, d, n)
    return m

a = int(io.recvline().split(b' = ')[1], 16)
b = int(io.recvline().split(b' = ')[1], 16)
n = int(io.recvline().split(b' = ')[1], 16)
w = int(io.recvline().split(b' = ')[1], 16)
e = int(io.recvline().split(b' = ')[1], 16)
c = int(io.recvline().split(b' = ')[1], 16)

print(a, b, n, w, e, c)
print(int(n).bit_length())
K = ZZ[i]
gaussian_factors = list(factor(K(w)))

for i in range(len(gaussian_factors)):
    gaussian_factors[i] = gaussian_factors[i][0]
print(gaussian_factors)

for i in range(1, len(gaussian_factors) + 1):
    for j in combinations(gaussian_factors, i):
        mul = prod(j)
        if is_prime(ZZ(abs(mul[0])) - ZZ(a)) and is_prime(ZZ(abs(mul[1])) - ZZ(b)):
            p, q = Integer(ZZ(abs(mul[0])) - ZZ(a)), Integer(ZZ(abs(mul[1])) - ZZ(b))
            break

assert is_prime(p)
assert is_prime(q)
assert p * q == n

print(long_to_bytes(int(decrypt(c, p, q, n))).decode())
