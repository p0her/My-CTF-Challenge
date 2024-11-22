from Crypto.Util.number import *

flag = open('./flag', 'rb').read()

p1, q1 = getStrongPrime(1024), getStrongPrime(1024)

N1 = p1 * q1

M = bytes_to_long(flag)
e1 = 15835
e2 = 34837
e3 = 65537

c1 = pow(M, e1, N1)
c2 = pow(M, e2, N1)
c3 = pow(M, e3, N1)

print(f'N1 = {N1}')
print(f'e1 = {e1}')
print(f'e2 = {e2}')
print(f'e3 = {e3}')

print(f'c1 = {c1}')
print(f'c2 = {c2}')
print(f'c3 = {c3}')
