#include "bits_operations.h"
#include  <stdio.h>

void print_bits(unsigned int bits) {
    int i = 0;
    for (i = 11; i >= 0; i--) {
        printf("%d", bits >> i & 1);
    }
}

void put_bits(int *bits, int val, int position) {
    *bits = *bits | val << position;
}

