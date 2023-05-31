
#include "c_test.h"

int count = 0;

void setAlarm() {
    int time;
    __asm__ ("csrr %0, 0xb01" : "=r" (time));
    time += 99000;
    __asm__ ("csrw 0xbc0, %0" : : "r" (time));
}

void mtvec_handler() {
    int cause;
    __asm__ ("csrr %0, mcause" : "=r" (cause));
    if ((cause & 0xf) == 7) {
        setAlarm();
        count++;
        if (count == 10) {
            leave(1);
        }
    }
    __asm__ ("mret");
}

void test() {
    setAlarm();
    __asm__ (
        "la t0, mtvec_handler;"
        "csrw mtvec, t0;"
        "li t1, (1 << 7);"
        "csrrs zero, mie, t1;"
        "li t1, (1 << 3);"
        "csrrs zero, mstatus, t1"
    );
    for (;;) {
        __asm__ ("wfi");
    }
}

