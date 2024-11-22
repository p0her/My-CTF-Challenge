from Crypto.Util.number import *
from fastecdsa.curve import secp256k1
from pwn import *
import hashlib

HOST = '3.34.91.236'
PORT = 20001

n = secp256k1.q

io = remote(HOST, PORT)

def play_game(message):
    io.sendlineafter('> ', '1')
    io.sendlineafter('message > ', message)
    signature_msg = io.recvline().split(b' : ')[1].strip()
    return eval(signature_msg)

def answer(secret):
    io.sendlineafter('> ', '2')
    io.sendlineafter('> ', secret)

r1, s1 = play_game('A')
z1 = int(hashlib.sha256(b'A').hexdigest(), 16)
r2, s2 = play_game('B')
z2 = int(hashlib.sha256(b'B').hexdigest(), 16)

k = ((z1-z2) * inverse(s1-s2, n)) % n

io.success(f'k : {hex(k)}')

answer(str(k))
flag = io.recvline().split(b'FLAG : ')[1].strip()
io.success(flag.decode())