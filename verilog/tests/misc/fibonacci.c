
#include "c_test.h"

int recursive(int n) {
    if (n <= 1) {
        return n;
    } else {
        return recursive(n - 1) + recursive(n - 2);
    }
}

void test() {
    ASSERT(2, recursive(10) == 55);
    ASSERT(3, recursive(15) == 610);
}

