#include <stdio.h>
#include <stdbool.h>

#include "millis.h"
#include "interval.h"

void spam_rdrand() {
    asm volatile(
        "mov $0x80, %%r9\n\t"
        "loop:\n\t"
        "rdrand %%r8\n\t"
        "dec %%r9\n\t"
        "jnz loop\n\t"
        :
        :
        : "r8", "r9"
    );
}

bool nth_bit(char* text_buf, int n) {
    int i = n / 8, j = n % 8;
    return text_buf[i] & (1 << (7 - j));
}

int main(int argc, char** argv) {
    printf("Please type a message.\n");
    bool sending = true;
    while (sending) {
        printf("MESSAGE: ");
        fflush(stdout);
        char raw_text_buf[128];
        fgets(raw_text_buf, sizeof(raw_text_buf), stdin);
        char text_buf[128];
        text_buf[0] = 'X';
        for (int i = 1; text_buf[i - 1] != '\0'; i++) {
            text_buf[i] = raw_text_buf[i - 1];
        }
        printf("Writing...\n");
        fflush(stdout);
        uint64_t start = (((micros() / INTERVAL) + 2) * INTERVAL);
        while (micros() < start) {}
        start += INTERVAL;
        while (micros() < start) {
            spam_rdrand();
        }
        start += INTERVAL / 2;
        for (int n = 0; text_buf[n / 8] != '\n'; n++) {
            bool bit = nth_bit(text_buf, n);
            printf("\r%d @ %d          ", bit, n);
            fflush(stdout);
            while (micros() < (start + (INTERVAL * n))) {
                if (bit) spam_rdrand();
            }
        }
        printf("\rDone.              \n");
    }
    return 0;
}
