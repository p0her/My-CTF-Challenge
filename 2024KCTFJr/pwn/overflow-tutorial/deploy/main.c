#include <stdio.h>
#include <stdlib.h>
#include <signal.h> // 시그널 헤더파일
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

void overflow_handler(int sig) {
    char buf[0x100];
    memset(buf, 0, 0x100);
    int fd = open("/flag", O_RDONLY);
    if(fd < 0) {
        perror("open error");
        exit(1);
    }
    read(fd, buf, 0x100);
    write(1, buf, 0x100);
    exit(0);
}

void initalization() {
    setvbuf(stdin, 0, _IONBF, 0);
    setvbuf(stdout, 0, _IONBF, 0);
    setvbuf(stderr, 0, _IONBF, 0);
}

int main() {
    char buf[0x100];
    signal(SIGSEGV, overflow_handler);
    initalization();
    printf("> ");
    read(0, buf, 0x200);
    return 0;
}