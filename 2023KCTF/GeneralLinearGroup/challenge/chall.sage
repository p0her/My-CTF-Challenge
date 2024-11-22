# SageMath version 9.5, Release Date: 2022-01-30
import random
from Crypto.Util.number import bytes_to_long
from secret import FLAG # FLAG is here
p = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F
assert is_prime(p)

N = 39
gl = GL(N, GF(p))
secret = matrix(gl.random_element())

val = 0
for i in range(N):
    for j in range(N):
        val += secret[i][j]

random.seed(int(val))

p, q = 0, 0
while True:
    p = random.randint(1, 1<<1024)
    if is_prime(p):
        break

while True:
    q = random.randint(1, 1<<1024)
    if is_prime(q):
        break

n = p * q
phi = (p - 1) * (q - 1)
e = 0x10001

c = pow(bytes_to_long(FLAG), e, n)

print(f'n = {n}')
print(f'e = {e}')
print(f'c = {c}')


