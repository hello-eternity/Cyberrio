
#include "c_test.h"

int isPrime(int n) {
    if (n <= 1) {
        return 0;
    } else {
        for (int i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                return 0;
            }
        }
        return 1;
    }
}

void test() {
    ASSERT(2, isPrime(2));
    ASSERT(3, isPrime(3));
    ASSERT(4, isPrime(5));
    ASSERT(5, !isPrime(4));
    ASSERT(6, !isPrime(1573));
    ASSERT(7, isPrime(1549));
    ASSERT(8, isPrime(3169));
}


