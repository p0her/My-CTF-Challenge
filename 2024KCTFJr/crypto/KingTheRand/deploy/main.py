from random import *
from Crypto.Util.number import *

flag = open('./flag', 'r').read()

p = getPrime(64)
A = getPrime(32)
B = getPrime(32)

def encrypt(x):
    return (A*x + B) % p

def encrypt2(x):
    return (getrandbits(32)*x**2 + getrandbits(32)*x + getrandbits(32))

print(p)
print(A)
print(B)

for i in range(624):
    print(encrypt(getrandbits(32)))

for c in flag:
    print(encrypt2(ord(c)))