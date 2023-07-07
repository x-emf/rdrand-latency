#ifndef __MEASURE_H__
#define __MEASURE_H__

#include <stdint.h>
#include <stdio.h>

uint64_t measure_rdrand_latency() {
    uint64_t sd, sa, ed, ea;

    asm volatile (
        "mfence\n\t"
        "lfence\n\t"
        "rdtscp\n\t"
        "mov %%rdx, %0\n\t"
        "mov %%rax, %1\n\t"
        "rdrand %%r8\n\t"
        "rdtscp\n\t"
        "mov %%rdx, %2\n\t"
        "mov %%rax, %3\n\t"
        : "=r"(sd), "=r"(sa), "=r"(ed), "=r"(ea)
        :
        : "r8", "%rax", "%rbx", "%rcx", "%rdx"
    );
    uint64_t start = (sd << 32) | sa;
    uint64_t end   = (ed << 32) | ea;
    return end - start;
}

#endif
