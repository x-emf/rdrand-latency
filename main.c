#include <stdio.h>

#include "measure.h"

int main(int argc, char** argv) {
    uint64_t latency = measure_rdrand_latency();
    printf("Latency in cycles: %ld\n", latency);
    return 0;
}
