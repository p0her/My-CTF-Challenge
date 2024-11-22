import random

FLAG = open('./flag.txt').read()

if __name__ == '__main__':
    x = int(input('x > '))
    y = str(input('y > '))

    if isinstance(x, int):
        random.seed(x, version=1)
        state1 = random.getrandbits(0x10001)
    if isinstance(y, str):
        random.seed(y, version=1)
        state2 = random.getrandbits(0x10001)

    if state1 == state2:
        print('FLAG : ' + FLAG)
    else:
        print('Wrong!')
        exit(1)