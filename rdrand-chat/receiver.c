#include <stdio.h>
#include <stdbool.h>

#include "millis.h"
#include "interval.h"
#include "../measure.h"

int main(int argc, char** argv) {
    int poll = ((millis() / INTERVAL) + 2) * INTERVAL;
    while (1) {
        while (millis() < poll) {}
        poll += INTERVAL;
        uint64_t latency = measure_rdrand_latency();
        bool bit = latency < 3100;
        printf("Latency in cycles: %ld, Measured bit: %d        \r", latency, bit);
        fflush(stdout);
    }
    return 0;
}
