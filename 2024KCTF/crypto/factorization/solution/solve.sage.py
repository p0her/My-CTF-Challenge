

# This file was *autogenerated* from the file solve.sage
from sage.all_cmdline import *   # import sage library

_sage_const_20002 = Integer(20002); _sage_const_0x10001 = Integer(0x10001); _sage_const_1 = Integer(1); _sage_const_16 = Integer(16); _sage_const_0 = Integer(0)
from itertools import combinations
from Crypto.Util.number import inverse, long_to_bytes
from pwn import *
REMOTE = True
if REMOTE:
    HOST, PORT = '54.180.153.92', int(_sage_const_20002 )
else:
    HOST, PORT = 'localhost', int(_sage_const_20002 )

io = remote(HOST, PORT)
context.log_level = 'debug'

def decrypt(c, p, q, n) -> int:
    e = _sage_const_0x10001 
    phi = (p - _sage_const_1 ) * (q - _sage_const_1 )
    d = inverse_mod(e, phi)
    m = power_mod(c, d, n)
    return m

a = int(io.recvline().split(b' = ')[_sage_const_1 ], _sage_const_16 )
b = int(io.recvline().split(b' = ')[_sage_const_1 ], _sage_const_16 )
n = int(io.recvline().split(b' = ')[_sage_const_1 ], _sage_const_16 )
w = int(io.recvline().split(b' = ')[_sage_const_1 ], _sage_const_16 )
e = int(io.recvline().split(b' = ')[_sage_const_1 ], _sage_const_16 )
c = int(io.recvline().split(b' = ')[_sage_const_1 ], _sage_const_16 )

print(a, b, n, w, e, c)
print(int(n).bit_length())
K = ZZ[i]
gaussian_factors = list(factor(K(w)))

for i in range(len(gaussian_factors)):
    gaussian_factors[i] = gaussian_factors[i][_sage_const_0 ]
print(gaussian_factors)

for i in range(_sage_const_1 , len(gaussian_factors) + _sage_const_1 ):
    for j in combinations(gaussian_factors, i):
        mul = prod(j)
        if is_prime(ZZ(abs(mul[_sage_const_0 ])) - ZZ(a)) and is_prime(ZZ(abs(mul[_sage_const_1 ])) - ZZ(b)):
            p, q = Integer(ZZ(abs(mul[_sage_const_0 ])) - ZZ(a)), Integer(ZZ(abs(mul[_sage_const_1 ])) - ZZ(b))
            break

assert is_prime(p)
assert is_prime(q)
assert p * q == n

print(long_to_bytes(int(decrypt(c, p, q, n))).decode())

