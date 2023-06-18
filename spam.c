#include <stdio.h>
#include <stdint.h>

int main(int argc, char** argv) {
    printf("Spamming rdrand...\n");
    asm volatile(
        "get:\n\t"
        "rdrand %%r8\n\t"
        "jmp get\n\t"
        :
        :
        : "r8"
    );
    return 0;
}
