from Crypto.Util.number import *
from fastecdsa.curve import secp256k1
import random
import hashlib

PLAY_GAME = 1
ANSWER = 2
EXIT = 3

encryptor_count = 0

class ECDSA:
    def __init__(self):
        self.G = secp256k1.G
        self.order = secp256k1.q
        self.priv = random.randint(1, self.order - 1)
        self.pub = self.G * self.priv
    
    def _sign(self, message):
        z = int(hashlib.sha256(message).hexdigest(), 16)
        k = self.priv
        r = (k * self.G).x
        s = (inverse(k, self.order) * (z + r * self.priv)) % self.order
        return (r, s)
    
    def _verify(self, message, r, s):
        if not (1 <= r <= self.order - 1 and 1 <= s <= self.order - 1):
            return False
        
        z = int(hashlib.sha256(message).hexdigest(), 16)

        u1 = (z * inverse(s, self.order)) % self.order
        u2 = (r * inverse(s, self.order)) % self.order

        P = u1 * self.G + u2 * self.pub
        return r == P.x

    def sign(self, message):
        return self._sign(message)

    def verify(self, message, r, s):
        return self._verify(message, r, s)

def get_flag():
    flag = open('flag', 'r').read()
    print(f'FLAG : {flag}')
    return

def start_menu():
    print('*** ECDSA Encryptor!! ***')
    print('1. Play Game')
    print('2. Answer')
    print('3. Exit')

def play_game(ecdsa):
    global encryptor_count
    if encryptor_count > 2:
        print('!!! can\'t more encrypt !!! ')
        return
    
    M = input('message > ').encode()
    r, s = ecdsa.sign(M)
    if ecdsa.verify(M, r, s):
        print(f'signature : {(r, s)}')
        encryptor_count += 1
    else:
        print('Failed verification')

def answer(ecdsa): 
    print('Tell me our secret')
    secret = int(input('> '))
    if secret == ecdsa.priv:
        get_flag()
    else:
        print('Wrong')
        exit(1)

def main():
    ecdsa = ECDSA()

    while True:
        start_menu()
        inp = int(input('> '))
        if inp == PLAY_GAME:
            play_game(ecdsa)
        elif inp == ANSWER:
            answer(ecdsa)
        elif inp == EXIT:
            exit(0)
        else:
            exit(0) 

if __name__ == '__main__':
    main()