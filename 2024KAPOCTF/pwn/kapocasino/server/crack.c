#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>

#define TemperingMaskB 0x9d2c5680
#define TemperingMaskC 0xefc60000
#define N 624
#define F 1812433253

uint32_t untemper(uint32_t y) {
    uint32_t y = undoTemperShiftL(y);
    y = undoTemperShitftT(y);
    y = undoTemperShiftS(y) ;
    y = undoTemperShiftU(y);
    return y;
}

uint32_t undoTempserShiftL(uint32_t y) {
    uint32_t last14 = y >> 18;
    uint32_t final = y ^ last14;
    return final;
}

uint32_t undoTemperShiftT(uint32_t y) {
    uint32_t first17 = y << 15;
    uint32_t final = y ^ (first17 & TemperingMaskC);
    return final;
}

uint32_t undoTemperShiftS(uint32_t y)  {
    uint32_t a = y << 17;
    uint32_t b = y ^ (a & TemperingMaskB);
    uint32_t c = b << 7;
    uint32_t d = y ^ (c & TemperingMaskB);
    uint32_t e = d << 7;
    uint32_t f = y ^ (e & TemperingMaskB);
    uint32_t g = f << 7;
    uint32_t h = y ^ (g & TemperingMaskB);
    uint32_t i = h << 7;
    uint32_t final = y ^ (i & TemperingMaskB);
    return final;
}

uint32_t undoTemperShiftU(uint32_t y) {
    uint32_t a = y >> 11;
    uint32_t b = y ^ a;
    uint32_t c = b >> 11;
    uint32_t final = y ^ c;
    return final;
}

static uint32_t mt[N];
static int mti = N + 1;

void init_genrand(uint32_t seed) {
    int _mti;
    mt[0] = seed;
    for(_mti = 1;  _mti < N; _mti++) {
        mt[_mti] = (F * (mt[_mti-1] ^ (mt[_mti-1] >> 30)) + _mti);
    }
    mti = _mti;
}