#include "bits_operations.h"
#include  <stdio.h>


/* print the bits that the num "bits" represent */
void print_bits(unsigned int bits) {
    int i = 0;
    for (i = 11; i >= 0; i--) {
        printf("%d", bits >> i & 1);
    }
}


/* put the value "val" in specific position in number "bits" (position starts from 0)
 * For example, if bits is 0, val is 3 (11 in binary base) and position is 2,
 * bits become to be 12 (1100 in binary base)*/
void put_bits(int *bits, int val, int position) {
    *bits = *bits | val << position;
}

