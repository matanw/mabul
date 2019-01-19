#include <stdlib.h>
#include <stdio.h>

enum my_type {
    INT, DOUBLE, FLOAT
};
typedef enum my_type MyType;
#define FIND_SUBSTRACTION(a, b, size, my_type){\
int i;\
my_type * res= (my_type *) malloc (size * sizeof(my_type));\
my_type *d=( my_type *)a;\
my_type *e=(my_type*) b;\
for(i=0;i<size;i++){\
res[i]=d[i]-e[i];\
}\
return (void*)res;\
}

void *find_subtraction(void *a, void *b, int size, MyType my_type) {
    if (my_type == INT) FIND_SUBSTRACTION(a, b, size, int)
    if (my_type == DOUBLE) FIND_SUBSTRACTION(a, b, size, double)
    if (my_type == FLOAT) FIND_SUBSTRACTION(a, b, size, float)
    return NULL;
}

/*
ב. לקוד של סעיף א נוסיף:
*/

int main() {
    int a[] = {1, 2, 3, 4, 5};
    int b[] = {6, 7, 8, 9, 10};
    int *c = find_subtraction(a, b, 5, INT);
    int i;
    for (i = 0; i < 5; i++)
        printf("%d\n", c[i]);
    free(c);
    return 0;
}


