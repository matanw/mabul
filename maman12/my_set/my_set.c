#include<stdio.h>
#include <stdlib.h>
#include "my_set.h"

/*prints a set.
 * params: set - the set to print
 */
void print_set(NumbersSet *set) {
    int i;
    printf("\nset is :");
    for (i = 0; i < set->size; i++) {
        printf("%d ", set->array[i]);
    }
}

/* Push number (num) to set*/
void push_to_set(NumbersSet *set, int num) {
    set->array[set->size] = num;
    set->size++;
}


/* Check if number is in a set, returns 1 if num is in set, otherwise, retruns 0*/
int is_in_set(NumbersSet *set, int num) {
    int i;
    for (i = 0; i < set->size; i++) {
        if (set->array[i] == num) {
            return 1;
        }
    }
    return 0;
}

/*push num to set, if num does not already exist in set*/
void push_if_not_exists(NumbersSet *set, int num) {
    if (!is_in_set(set, num)) {
        push_to_set(set, num);
    }
}

/*initialize a set*/
NumbersSet *init_set() {
    NumbersSet *set = (NumbersSet *) malloc(sizeof(NumbersSet));
    set->size = 0;
    return set;
}

/*gets number from standard input, and puts them in set, returns the set*/
NumbersSet *get_set() {
    NumbersSet *set = init_set();
    int num;
    while (scanf("%d", &num) == 1) {
        printf("%d ", num);
        push_if_not_exists(set, num);
    }
    return set;
}

int main() {
    NumbersSet *set = get_set();
    print_set(set);
    free(set);
    return 0;

}
