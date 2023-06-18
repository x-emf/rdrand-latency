#ifndef __MEASURE_H__
#define __MEASURE_H__

#include <stdint.h>

uint64_t measure_rdrand_latency() {
    uint64_t cycles;

    asm volatile (
        "mfence\n\t"
        "lfence\n\t"
        "rdtscp\n\t"
        "mov %%eax, %%edi\n\t"
        "rdrand %%r8\n\t"
        "rdtscp\n\t"
        "sub %%edi, %%eax"
        : "=a"(cycles)
        :
        : "r8", "edi"
    );

    return cycles;
}

#endif
