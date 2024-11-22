from pwn import *
REMOTE = True
if REMOTE:
    HOST, PORT = '54.180.153.92', int(20000)
else:
    HOST, PORT = 'localhost', int(20000)
io = remote(HOST, PORT)

p = 251
F = GF(p)

R.<x> = PolynomialRing(F)

degree = 128
Cx = eval(io.recvline().replace(b'^', b'**'))
Nx = eval(io.recvline().replace(b'^', b'**'))

fact = Nx.factor()
Px, Qx = fact[0][0], fact[1][0]
print(f'Px : {Px}')
print(f'Qx : {Qx}')

assert Px*Qx == Nx

phi = (p**Px.degree() - 1) * (p**Qx.degree() - 1)
e = 0x10001
d = inverse_mod(e, phi)

plain = pow(Cx, d, Nx)
flag = b''
for x in plain:
    flag += bytes([x])
print(flag.decode())
