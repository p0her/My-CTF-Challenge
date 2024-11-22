#!/usr/bin/env python3
from qiling import Qiling
import sys
if __name__ == '__main__':
    if len(sys.argv) < 2:
        print(f'Usage: {sys.argv[0]} <ELF>')
        sys.exit(1)
    
    ql = Qiling(['./lib/ld-2.31.so', '--library-path', '/lib', sys.argv[1]], console=False, rootfs='.')
    ql.run()
