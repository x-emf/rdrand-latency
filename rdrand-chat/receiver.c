#include <stdio.h>
#include <stdbool.h>

#include "millis.h"
#include "interval.h"
#include "../measure.h"

#define LATENCY_SAMPLES 10

int main(int argc, char** argv) {
    struct timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = 5 * 1000 * 1000;
    int poll = ((millis() / INTERVAL) + 2) * INTERVAL;
    bool reading = false;
    while (1) {
        while (millis() < poll) {}
        poll += INTERVAL;
        int num_high = 0;
        for (int sample = 0; sample < LATENCY_SAMPLES; sample++) {
            uint64_t latency = measure_rdrand_latency();
            nanosleep(&delay, &delay);
            num_high += latency < 2900;
        }
        bool bit = num_high > (LATENCY_SAMPLES / 2);
        //printf("%d/%d high -> Measured bit: %d        \r", num_high, LATENCY_SAMPLES, bit);
        //fflush(stdout);
        if (reading) {
            printf("Read bit: %d\n", bit);
        } else {
            if (bit) {
                printf("Read pre bit. \n");
                printf("=== START MESSAGE ===\n");
                reading = true;
            }
        }
    }
    return 0;
}
