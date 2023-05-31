
#include "c_test.h"

int partition(int n) {
    int pent[n + 1];
    pent[0] = 1;
    int pent_rec[n + 1];
    pent_rec[0] = 1;
    int pent_rec_count = 1;
    for (int i = 1; i <= n; i++) {
        int next_value = 0;
        while (pent_rec[pent_rec_count - 1] < i) {
            if (pent_rec_count % 2 == 1) {
                pent_rec[pent_rec_count] = pent_rec[pent_rec_count - 1] + (pent_rec_count - 1) / 2 + 1;
                pent_rec_count += 1;
            } else {
                pent_rec[pent_rec_count] = pent_rec[pent_rec_count - 1] + pent_rec_count + 1;
                pent_rec_count += 1;
            }
        }
        for (int j = 0; j < pent_rec_count && pent_rec[j] <= i; j++) {
            if (j / 2 % 2 == 0) {
                next_value += pent[i - pent_rec[j]];
            } else {
                next_value -= pent[i - pent_rec[j]];
            }
        }
        pent[i] = next_value;
    }
    return pent[n];
}

void test() {
    ASSERT(2, partition(41) == 44583);
    ASSERT(3, partition(49) == 173525);
    ASSERT(4, partition(77) == 10619863);
}

