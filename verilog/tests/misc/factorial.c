
#include "c_test.h"

long long iterative(int n) {
    long long r = 1;
    for (int i = 1; i <= n; i++) {
        r *= i;
    }
    return r;
}

void test() {
    ASSERT(2, iterative(16) == 20922789888000LL);
}

