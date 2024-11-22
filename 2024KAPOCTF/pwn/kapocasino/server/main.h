#include <x86_64-linux-gnu/asm/unistd_64.h>
#include <stdint.h>
#include <stdio.h>
#include <openssl/md5.h>
#include <stdint.h>
#include <string.h> 
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/random.h>
#include <unistd.h> 

#define PAGE_SIZE 0x1000
#define N 624
#define M 397
#define F 1812433253U
#define MATRIX_A 0x9908b0dfU
#define UPPER_MASK 0x80000000U
#define LOWER_MASK 0x7fffffffU
#define W 32
#define R 31

void init_genrand(uint32_t seed);
uint32_t genrand_uint32();
uint32_t get_random();