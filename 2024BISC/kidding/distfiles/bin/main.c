#include <stdio.h>
void init() {
    setvbuf(stdin, 0, 2, 0); 
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
}
int main() {
    init();
    char buf[0x100];
    scanf("%s", buf);
    return 0;
}