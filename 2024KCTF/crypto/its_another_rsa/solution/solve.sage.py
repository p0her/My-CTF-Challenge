

# This file was *autogenerated* from the file solve.sage
from sage.all_cmdline import *   # import sage library

_sage_const_20000 = Integer(20000); _sage_const_251 = Integer(251); _sage_const_128 = Integer(128); _sage_const_0 = Integer(0); _sage_const_1 = Integer(1); _sage_const_0x10001 = Integer(0x10001)
from pwn import *
REMOTE = True
if REMOTE:
    HOST, PORT = '54.180.153.92', int(_sage_const_20000 )
else:
    HOST, PORT = 'localhost', int(_sage_const_20000 )
io = remote(HOST, PORT)

p = _sage_const_251 
F = GF(p)

R = PolynomialRing(F, names=('x',)); (x,) = R._first_ngens(1)

degree = _sage_const_128 
Cx = eval(io.recvline().replace(b'^', b'**'))
Nx = eval(io.recvline().replace(b'^', b'**'))

fact = Nx.factor()
Px, Qx = fact[_sage_const_0 ][_sage_const_0 ], fact[_sage_const_1 ][_sage_const_0 ]
print(f'Px : {Px}')
print(f'Qx : {Qx}')

assert Px*Qx == Nx

phi = (p**Px.degree() - _sage_const_1 ) * (p**Qx.degree() - _sage_const_1 )
e = _sage_const_0x10001 
d = inverse_mod(e, phi)

plain = pow(Cx, d, Nx)
flag = b''
for x in plain:
    flag += bytes([x])
print(flag.decode())

