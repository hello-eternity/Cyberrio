
#include "c_test.h"

int ackermann(int m, int n) {
    if (m == 0) {
        return n + 1;
    } else if (n == 0) {
        return ackermann(m - 1, 1);
    } else {
        return ackermann(m - 1, ackermann(m, n - 1));
    }
}

void test() {
    ASSERT(2, ackermann(0, 0) == 1);
    ASSERT(3, ackermann(0, 1) == 2);
    ASSERT(4, ackermann(1, 2) == 4);
    ASSERT(5, ackermann(2, 3) == 9);
    ASSERT(6, ackermann(3, 2) == 29);
}

