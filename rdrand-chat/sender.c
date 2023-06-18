#include <stdio.h>
#include <stdbool.h>

#include "millis.h"
#include "interval.h"

void spam_rdrand() {
    asm volatile(
        "mov $0x100, %%r9\n\t"
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
        char text_buf[128];
        fgets(text_buf, sizeof(text_buf), stdin);
        printf("Writing...");
        fflush(stdout);
        uint64_t start = (((millis() / INTERVAL) + 2) * INTERVAL);
        while (millis() < start) {}
        start += INTERVAL;
        while (millis() < start) spam_rdrand();
        for (int n = 0; text_buf[n / 8] != '\n'; n++) {
            bool bit = nth_bit(text_buf, n);
            printf("\r%d @ %d          ", bit, n);
            fflush(stdout);
            while (millis() < (start + (INTERVAL * n))) {
                if (bit) spam_rdrand();
            }
        }
        printf("\rDone.           \n");
    }
    return 0;
}
