#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

unsigned long long seed;
unsigned long long multiplier;
unsigned long long constant;
unsigned long long state;
uint8_t is_memory_maps = 0;

char name[]="whitehat master";
void init_buf() {
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stderr, 0, 2, 0);
}

unsigned long long get_lcg() {
    state = (state * multiplier + constant) & ((1<<64) - 1);
    return state;
}

void init_lcg() {
    int fd = open("/dev/urandom", O_RDONLY);
    if(fd == -1) {
        puts("Can't open /dev/urandom");
        exit(1);
    }
    read(fd, &seed, 4);
    read(fd, &multiplier, 4);
    read(fd, &constant, 4);
    srand(seed);
    state = rand();
    close(fd);
}

void get_memory_maps() {
    if(is_memory_maps) {
        printf("Chance is once\n");
        return;
    }
    char maps[512];
    FILE* stream = fopen("/proc/self/maps", "r");
    if(!stream) { 
        puts("Can't open /proc/self/maps");
        exit(1);
    }
    memset(maps, 0, 512);
    unsigned long long start_addr;
    unsigned long long end_addr;
    char permission[5];
    printf("setting state...\n");
    for(int i = 0; i < 4; i++) 
        printf("current state : %llu\n", get_lcg()); 
    while(1) { 
        if(fscanf(stream, "%llx-%llx %4s", &start_addr, &end_addr, permission) != -1) {
            fgets(maps, 512, stream);
            if(strstr(permission, "w") && !strstr(maps, "heap") && !strstr(maps, "stack") ) 
                printf("addr : 0x%llx\n", (start_addr * multiplier + constant) & ((1<<64) - 1));
            
            
        } else break;
    }
    is_memory_maps = 1;
    fclose(stream);
}

void print_menu() {
    puts("1. Get memory maps 🗺️"); 
    puts("2. write memory 📖");
    puts("3. Exit");
}

void write_memory() {
    unsigned long long addr;
    printf("addr > ");
    scanf("%llu", &addr);
    read(0, (void*)addr, 8);
}

int main() {
    init_buf();
    init_lcg();
    puts("[*] LCG initialize success");
    while(1) {
        print_menu();
        int case_num; 
        printf("> "); scanf("%d", &case_num);
        switch(case_num) {
            case 1:
                get_memory_maps();
                break;
            case 2:
                write_memory();
                break;
            case 3:
                printf("Bye ");
                puts(name);
                exit(0);
                break;
            default:
                puts("Wrong Input");
        }
    }
}