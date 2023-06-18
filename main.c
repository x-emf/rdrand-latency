#include <stdio.h>
#include <time.h>

#include "measure.h"

int main(int argc, char** argv) {
    struct timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = 50 * 1000 * 1000;
    while (1) {
        uint64_t latency = measure_rdrand_latency();
        printf("Latency in cycles: %ld\n", latency);
        nanosleep(&delay, &delay);
    }
    return 0;
}
