#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

#define FD_MX 65536
#define MX_BUF_SIZE 0x100
#define STDOUT 1

void initalization() {
    setvbuf(stdin, 0, _IONBF, 0);
    setvbuf(stdout, 0, _IONBF, 0);
    setvbuf(stderr, 0, _IONBF, 0);
}

int main() {
    initalization();
    char buf[MX_BUF_SIZE];
    memset(buf, 0x0, MX_BUF_SIZE);
    unsigned int idx;
    srand(time(0));
    unsigned int fd = (rand() % (FD_MX - 5 + 1)) + 5;
    for(int i = 5; i < FD_MX; i++)
        open("/flag", O_RDONLY);
    
    for(int i = 5; i < FD_MX; i++)
        if(i != fd) close(i);
    
    printf("> ");
    scanf("%u", &idx);
    int ret = read(idx, buf, MX_BUF_SIZE);
    if(ret < 0) {
        perror("read");
        exit(1);
    }
    write(STDOUT, buf, MX_BUF_SIZE);
    close(fd);
}