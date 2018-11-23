#define MAX_DIFFERENT_NUMBERS_COUNT 64
typedef struct numbers_set {
    int array[MAX_DIFFERENT_NUMBERS_COUNT];
    int size;
} NumbersSet;

void print_set(NumbersSet *);

void push_to_set(NumbersSet *, int);

int is_in_set(NumbersSet *, int);

void push_if_not_exists(NumbersSet *, int);

NumbersSet *init_set();

NumbersSet *get_set();
