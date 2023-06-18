#include <stdio.h>
#include <stdbool.h>

#include "millis.h"
#include "interval.h"
#include "../measure.h"

#define LATENCY_SAMPLES 4

int main(int argc, char** argv) {
    struct timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = 50 * 1000;
    int poll = ((millis() / INTERVAL) + 2) * INTERVAL;
    bool reading = false;
    int read_index = 0;
    char read_buf[128] = {'\0'};
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
            printf("(read %d)\n", bit);
            if (bit) read_buf[read_index / 8] |= (1 << (7 - (read_index % 8)));
            if ((read_index > 7) && (read_buf[(read_index / 8) - 1] == '\0')) {
                reading = false;
                read_index = 0;
                printf("Got: %s\n", read_buf + 1);
                for (int i = 0; i < sizeof(read_buf)/sizeof(char); i++) {
                    read_buf[i] = '\0';
                }
            } else {
                read_index++;
            }
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
