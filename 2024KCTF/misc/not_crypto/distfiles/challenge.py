import random

FLAG = open('./flag.txt').read()

def stage1():
    x = int(input('x > '))
    y = str(input('y > '))

    if isinstance(x, int):
        random.seed(x, version=1)
        state1 = random.getrandbits(0x10001)
    if isinstance(y, str):
        random.seed(y, version=1)
        state2 = random.getrandbits(0x10001)

    if state1 == state2:
        print('Stage1 done')
        return
    else:
        print('Wrong!')
        exit(1)

def stage2():
    x = int(input('x > '))
    y = str(input('y > '))

    if isinstance(x, int):
        random.seed(x)
        state1 = random.getrandbits(0x10001)
    if isinstance(y, str):
        random.seed(y)
        state2 = random.getrandbits(0x10001)
    
    if state1 == state2:
        print('FLAG : ' + FLAG)
    else:
        print('Wrong!')

if __name__ == '__main__':
    stage1()
    stage2()