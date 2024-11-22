#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define ENCRYPT 1
#define EXIT 2
#define MAX_SIZE 0x100

void initalization() {
    setvbuf(stdin, 0, _IONBF, 0);
    setvbuf(stdout, 0, _IONBF, 0);
    setvbuf(stderr, 0, _IONBF, 0);
}

void encrypt(unsigned char* pt, unsigned char* ct) {
    memset(pt, 0, MAX_SIZE);
    memset(ct, 0, MAX_SIZE);
    printf("plaintext : ");
    char tmp[0x401];
    memset(tmp, 0, 0x400);
    read(0, pt, 0x200);
    size_t len = strlen(pt);
    for(int i = 0; i < len; i++)
        ct[i] = (pt[i] + 3) & 0xff;
    for(int i = 0; i < len; i++)
        sprintf(&tmp[2*i], "%02x", ct[i]);
    printf("ciphertext : %s\n", tmp);
}

void print_menu() {
    puts("1. encrypt");
    puts("2. exit");
    printf("> ");
}

int main() {
    unsigned char pt[MAX_SIZE];
    unsigned char ct[MAX_SIZE];
    initalization();
    while(1) {
        int sel;
        print_menu();
        scanf("%d", &sel);
        switch(sel) {
            case ENCRYPT:
                encrypt(pt, ct);
                break;
            case EXIT:
                return 0;
            default:
                break;
        }
    }
}