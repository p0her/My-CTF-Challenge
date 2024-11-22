#include <unicorn/unicorn.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define PROGRAM_SIZE 0x6000
#define BUF_SIZE 0x100
#define STACK_SIZE 0x100
#define ADDRESS 0x800000
#define MAX_FILE 3

#define MOV 0x0
#define LEA 0x1
#define ADD 0x2
#define SUB 0x3
#define MUL 0x4
#define DIV 0x5
#define PUSH 0x6
#define POP 0x7
#define SHL 0x8
#define SHR 0x9
#define AND 0xA
#define OR 0xB
#define XOR 0xC

#define READ 0x1000
#define WRITE 0x1001
#define CLOSE 0x1002
#define OPEN 0x1003

#define MAX_REG 6

#define FILE_READ 0
#define BUFFER_READ 1
#define FILE_WRITE 0
#define BUFFER_WRITE 1

#define STDIN 0
#define STDOUT 1

struct _file
{
    FILE *fp;
    unsigned char buf[BUF_SIZE];
    size_t len;
};

unsigned char stack[STACK_SIZE];
int rsp;
int is_open;
uint64_t reg[MAX_REG];
unsigned char program[PROGRAM_SIZE];
struct _file _FILE[MAX_FILE] = {
    {.len = BUF_SIZE},
    {.len = BUF_SIZE},
    {.len = BUF_SIZE}};

int check_reg_range(uint64_t reg)
{
    if (reg < 0 || reg >= MAX_REG)
        return 0;
    return 1;
}

void readline(unsigned char *buf, size_t len)
{
    size_t idx = 0;
    size_t n;
    while (idx <= len)
    {
        if ((n = read(STDIN, &buf[idx], 1)) <= 0)
            perror("read");
        if (buf[idx] == '\n')
            break;
        ++idx;
    }
}

int filter(char *filename)
{
    if (strstr(filename, "flag"))
        return 1;
    if (strstr(filename, "proc"))
        return 1;
    return 0;
}
void hook_syscall(uc_engine *uc, void *user_data)
{
    uint64_t rax;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t rcx;
    int val = 0;
    uc_reg_read(uc, UC_X86_REG_RAX, &rax);
    uc_reg_read(uc, UC_X86_REG_RDI, &rdi);
    uc_reg_read(uc, UC_X86_REG_RSI, &rsi);
    uc_reg_read(uc, UC_X86_REG_RDX, &rdx);
    uc_reg_read(uc, UC_X86_REG_RCX, &rcx);

    uc_reg_write(uc, UC_X86_REG_RAX, &val);
    uc_reg_write(uc, UC_X86_REG_RDI, &val);
    uc_reg_write(uc, UC_X86_REG_RSI, &val);
    uc_reg_write(uc, UC_X86_REG_RDX, &val);
    uc_reg_write(uc, UC_X86_REG_RCX, &val);
    uc_reg_write(uc, UC_X86_REG_R15, &val);
    uc_reg_write(uc, UC_X86_REG_R14, &val);
    uc_reg_write(uc, UC_X86_REG_R8, &val);
    uc_reg_write(uc, UC_X86_REG_R9, &val);
    uc_reg_write(uc, UC_X86_REG_R10, &val);
    uc_reg_write(uc, UC_X86_REG_R11, &val);
    uc_reg_write(uc, UC_X86_REG_R12, &val);
    uc_reg_write(uc, UC_X86_REG_R13, &val);
    switch (rax)
    {
    case MOV:
        if (!check_reg_range(rsi))
            return;
        if (rdi == 0)
            reg[rsi] = rdx;
        if (rdi == 1)
        {
            if (!check_reg_range(rdx))
                return;
            reg[rsi] = reg[rdx];
        }
        break;

    case LEA:
        if (!check_reg_range(rsi))
            return;
        if (rdi == 0) // mode
        {
            if (0 <= reg[rsi] && reg[rsi] < STACK_SIZE)
                *(uint64_t *)(stack + reg[rsi]) = rdx;
        }
        if (rdi == 1)
        {
            if (!check_reg_range(rdx))
                return;
            if (0 <= reg[rsi] && reg[rsi] < STACK_SIZE)
                *(uint64_t *)(stack + reg[rsi]) = reg[rdx];
        }
        break;

    case ADD:
        if (rdi == 0)
        {
            if (!check_reg_range(rsi))
                return;
            reg[rsi] += rdx;
        }
        if (rdi == 1)
        {
            if (!check_reg_range(rsi) || !check_reg_range(rdx))
                return;
            reg[rsi] += reg[rdx];
        }
        break;

    case SUB:
        if (rdi == 0)
        {
            if (!check_reg_range(rsi))
                return;
            reg[rsi] -= rdx;
        }
        if (rdi == 1)
        {
            if (!check_reg_range(rsi) || !check_reg_range(rdx))
                return;
            reg[rsi] -= reg[rdx];
        }
        break;

    case MUL:
        if (rdi == 0)
        {
            if (!check_reg_range(rsi))
                return;
            reg[rsi] *= rdx;
        }
        if (rdi == 1)
        {
            if (!check_reg_range(rsi) || !check_reg_range(rdx))
                return;
            reg[rsi] *= reg[rdx];
        }
        break;

    case DIV:
        if (rdi == 0)
        {
            if (!check_reg_range(rsi) || rdx == 0) // divided by zero
                return;
            reg[rsi] /= rdx;
        }
        if (rdi == 1)
        {
            if (!check_reg_range(rsi) || !check_reg_range(rdx) || reg[rdx] == 0) // divided by zero
                return;
            reg[rsi] /= reg[rdx];
        }
        break;

    case SHL:

        if (rdi == 0)
        {
            if (!check_reg_range(rsi))
                return;
            reg[rsi] <<= rdx;
        }
        if (rdi == 1)
        {
            if (!check_reg_range(rsi) || !check_reg_range(rdx))
                return;
            reg[rsi] <<= reg[rdx];
        }
        break;

    case SHR:
        if (rdi == 0)
        {
            if (!check_reg_range(rsi))
                return;
            reg[rsi] >>= rdx;
        }
        if (rdi == 1)
        {
            if (!check_reg_range(rsi) || !check_reg_range(rdx))
                return;
            reg[rsi] >>= reg[rdx];
        }
        break;

    case AND:
        if (rdi == 0)
        {
            if (!check_reg_range(rsi))
                return;
            reg[rsi] &= rdx;
        }
        if (rdi == 1)
        {
            if (!check_reg_range(rsi) || !check_reg_range(rdx))
                return;
            reg[rsi] &= reg[rdx];
        }
        break;

    case OR:
        if (rdi == 0)
        {
            if (!check_reg_range(rsi))
                return;
            reg[rsi] |= rdx;
        }
        if (rdi == 1)
        {
            if (!check_reg_range(rsi) || !check_reg_range(rdx))
                return;
            reg[rsi] |= reg[rdx];
        }
        break;

    case XOR:
        if (rdi == 0)
        {
            if (!check_reg_range(rsi))
                return;
            reg[rsi] ^= rdx;
        }
        if (rdi == 1)
        {
            if (!check_reg_range(rsi) || !check_reg_range(rdx))
                return;
            reg[rsi] ^= reg[rdx];
        }
        break;

    case PUSH:
        if (rsp >= STACK_SIZE) // full
            return;
        stack[rsp++] = reg[rdi] & 0xff;
        break;
    case POP:
        reg[rdi] = stack[--rsp];
        break;
    case OPEN:
        if (rdi < 0 || rdi >= STACK_SIZE)
            return;
        if (rsi >= MAX_FILE)
            return;
        if (filter((char *)&stack[rdi]))
            return;
        if (is_open)
            return;
        is_open = 1;
        _FILE[rsi].fp = fopen((char *)&stack[rdi], "w+");
        break;
    case READ:
        unsigned char c;
        if (rsi >= MAX_FILE)
            return;
        if (rdi == FILE_READ && _FILE[rsi].fp != NULL)
        {
            rewind(_FILE[rsi].fp);
            while ((c = fgetc(_FILE[rsi].fp)) != (unsigned char)EOF)
                putchar(c);
        }
        if (rdi == BUFFER_READ)
            write(STDOUT, _FILE[rsi].buf, _FILE[rsi].len);
        break;
    case WRITE:
        if (rsi >= MAX_FILE)
            return;
        if (rdi == FILE_WRITE && _FILE[rsi].fp != NULL)
        {
            rewind(_FILE[rsi].fp);
            fputs(stack, _FILE[rsi].fp);
        }
        if (rdi == BUFFER_WRITE)
            readline(_FILE[rsi].buf, _FILE[rsi].len);
        break;
    case CLOSE:
        if (rdi >= MAX_FILE)
            return;
        if (_FILE[rdi].fp != NULL)
            fclose(_FILE[rdi].fp);
        break;
    default:
        write(STDOUT, "[!] invalid instruction.", 25);
        break;
    }
}

void vm(unsigned char *program)
{
    uc_engine *uc;
    uc_err err;
    uc_hook trace1;
    err = uc_open(UC_ARCH_X86, UC_MODE_64, &uc);
    uc_mem_map(uc, ADDRESS, 2 * 1024 * 1024, UC_PROT_ALL);
    uc_mem_write(uc, ADDRESS, program, PROGRAM_SIZE);
    uc_hook_add(uc, &trace1, UC_HOOK_INSN, hook_syscall, NULL, 1, 0,
                UC_X86_INS_SYSCALL);
    err = uc_emu_start(uc, ADDRESS, ADDRESS + PROGRAM_SIZE, 0,
                       0);
}
void init()
{
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
}
int ofs = 0, ret = 0;
int main(int argc, char **argv)
{
    init();
    write(1, "opcode > ", 10);
    while (ofs < PROGRAM_SIZE)
    {
        ret = read(0, program + ofs, PROGRAM_SIZE - ofs);
        if (ret < 0)
            return 1;
        ofs += ret;
    }
    write(1, "**** run program ****\n", 23);
    vm(program);
}