import torch

if __name__ == '__main__':
    line = input('> ')
    loc = int(input('> '))
    torch.jit.annotations.parse_type_line(line, None, loc)