
#define EXIT_ADDRESS 0x11000000U

#define ASSERT(NUM, COND) { if (!(COND)) { leave(NUM); } }

void test();
void leave(int code);

void _start() {
    __asm__ ("li sp, 0x80100000");
    test();
    leave(1);
}

void leave(int code) {
    for (;;) {
        (*(volatile int*)EXIT_ADDRESS) = code;
    }
}

#include "c_support.h"

