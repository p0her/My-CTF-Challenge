#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 0x100
#define STDIN 0

const char *command = "/bin/sh\x00";

void stage1() {
    __asm__ __volatile__("mov command, %rdi");
}

void stage2() {
    __asm__ __volatile__("mov $0, %rsi");
}

void stage3() {
    __asm__ __volatile__("mov $0, %rdx");
}

void win(const char *pathname, char const *argv[], char *const envp[]) {
    execve(pathname, argv, envp);
}

void initalization() {
    setvbuf(stdin, 0, _IONBF, 0);
    setvbuf(stdout, 0, _IONBF, 0);
    setvbuf(stderr, 0, _IONBF, 0);
}

void foo() {
    char buf[BUF_SIZE];
    read(STDIN, buf, BUF_SIZE + 0x8 + 0x8);
}

int main() {
    initalization();
    foo();
}