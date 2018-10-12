#include "bits_operations.h"
#include  <stdio.h>

void print_bits_chars(unsigned char bits[2]) {
    print_bits_inner_chars(bits[1]);
    printf(",");
    print_bits_inner_chars(bits[0]);
    printf("\n");
}

void print_bits_inner_chars(unsigned char bits) {
    int i = 0;
    for (i = 5; i >= 0; i--) {
        printf("%d", bits >> i & 1);
    }
}

void print_bits_int(unsigned int bits) {
    int i = 0;
    for (i = 11; i >= 0; i--) {
        printf("%d", bits >> i & 1);
    }
}

void put_bits_int(int *bits, int val, int position) {
    *bits = *bits | val << position;
}

