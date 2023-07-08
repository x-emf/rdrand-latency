#include <stdio.h>
#include <stdbool.h>

#include "millis.h"
#include "interval.h"
#include "../measure.h"

#define LATENCY_SAMPLES 5

int main(int argc, char** argv) {
    struct timespec delay;
    delay.tv_sec = 0;
    delay.tv_nsec = 5 * 1000;
    uint64_t poll = ((micros() / INTERVAL) + 2) * INTERVAL;
    bool reading = false;
    int read_index = 0;
    char read_buf[128] = {'\0'};
    char partial_buf[128] = {'\0'};
    while (1) {
        while (micros() < poll) {}
        poll += INTERVAL;
        int num_high = 0;
        uint64_t latency;
        for (int sample = 0; sample < LATENCY_SAMPLES; sample++) {
            latency = measure_rdrand_latency();
            nanosleep(&delay, &delay);
            num_high += latency < 2900;
        }
        bool bit = num_high > (LATENCY_SAMPLES / 2);
        if (reading) {
            if (bit) {
                read_buf[read_index / 8] |= (1 << (7 - (read_index % 8)));
            }
            if (read_index > 0 && ((read_index % 8) == 0)) {
                int partial_update_index = (read_index - 1) / 8;
                partial_buf[partial_update_index] = read_buf[partial_update_index];
            }
            printf("\r%d/%d -> 1'b%d: %s\r", num_high, LATENCY_SAMPLES, bit, partial_buf + 1);
            fflush(stdout);
            if ((read_index > 7) && (read_buf[(read_index / 8) - 1] == '\0')) {
                reading = false;
                read_index = 0;
                printf("\rGOT MESSAGE: %s                                           ", read_buf + 1);
                fflush(stdout);
                if (read_buf[1] == '\0') {
                    printf("\r                                                              \r");
                    fflush(stdout);
                } else {
                    printf("\n");
                }
                for (int i = 0; i < sizeof(read_buf)/sizeof(char); i++) {
                    read_buf[i] = '\0';
                    partial_buf[i] = '\0';
                }
            } else {
                read_index++;
            }
        } else {
            printf("Latency: %ld | %d/%d high -> Measured bit: %d        \r", latency, num_high, LATENCY_SAMPLES, bit);
        fflush(stdout);
            if (bit) {
                reading = true;
            }
        }
    }
    return 0;
}
