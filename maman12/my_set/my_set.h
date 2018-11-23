#define MAX_DIFFERENT_NUMBERS_COUNT 64
typedef struct numbers_set {
    int array[MAX_DIFFERENT_NUMBERS_COUNT];
    int size;
} NumbersSet;

/* Prints a set.
 * params: set - the set to print
 */
void print_set(NumbersSet *);


/* Insert number (num) to set
 * params: set -set to insert to
 *         num -num to insert*/
void insert_to_set(NumbersSet *set, int num);


/* Check if number is in a set
 *
 *
 * params: set -set to check number i
 *         num -num to check*
 * return value: 1 if num is in set, otherwise, returns 0*/
int is_in_set(NumbersSet *set, int num);


/* Initialize a set*/
NumbersSet *init_set();


/* Get number from standard input, and puts them in set
 *
 * params: set -set to insert the scanned numbers to
 * */
void get_set(NumbersSet *set);

