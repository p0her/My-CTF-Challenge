#include <stdio.h>
#include <stdlib.h>

int slot[0x10];

void win() // call this!!
{
    system("cat /flag");
    exit(1);
}

void init()
{
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
}

int main()
{
    int idx, val;
    init();
    printf("slot idx > ");
    scanf("%d", &idx);
    if (idx >= 0x10)
    {
        puts("[!] out of bound");
        exit(1);
    }

    printf("slot value > ");
    scanf("%d", &val);
    slot[idx] = val;
    puts("[*] Done!");
}