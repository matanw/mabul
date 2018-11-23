#include<stdio.h>
#include <stdlib.h>
#include "my_set.h"


void print_set(NumbersSet *set) {
    int i;
    printf("The set is :");
    for (i = 0; i < set->size; i++) {
        printf("%d ", set->array[i]);
    }
}

void insert_to_set(NumbersSet *set, int num) {
    if (!is_in_set(set, num)) {
        set->array[set->size] = num;
        set->size++;
    }
}


int is_in_set(NumbersSet *set, int num) {
    int i;
    for (i = 0; i < set->size; i++) {
        if (set->array[i] == num) {
            return 1;
        }
    }
    return 0;
}

NumbersSet *init_set() {
    NumbersSet *set = (NumbersSet *) malloc(sizeof(NumbersSet));
    set->size = 0;
    return set;
}

void get_set(NumbersSet *set) {
    int num;
    while (scanf("%d", &num) == 1) {
        printf("Number was scanned:%d\n", num);
        insert_to_set(set, num);
    }
}

int main() {
    NumbersSet *set = init_set();
    get_set(set);
    print_set(set);
    free(set);
    return 0;

}
