#include  <stdio.h>
#include  <stdlib.h>

int specific_int(int *arr, int size) {
    int i;
    int sum = 0;
    for (i = 0; i < size; i++)
        sum += arr[i];
    return sum;
}

enum numeric_type {
    INT = 1, DOUBLE = 2, FLOAT = 3
};

void *generic_by_switch(void *arr, int size, enum numeric_type type) {
    int i;
    switch (type) {
        case INT: {
            int *sum = (int *) malloc(sizeof(int));
            *sum = 0;
            int *a = (int *) arr;
            for (i = 0; i < size; i++)
                *sum += a[i];
            return sum;
        }
        case DOUBLE: {
            double *sum = (double *) malloc(sizeof(double));
            *sum = 0;
            double *a = (double *) arr;
            for (i = 0; i < size; i++)
                *sum += a[i];
            return sum;
        }
        default:
            return 2;//todo:
    }
}

#define  GET_SUM(type) { \
type *sum = (type *) malloc(sizeof(type)); \
*sum = 0; \
type *a = (type *) arr; \
for (i = 0; i < size; i++) \
*sum += a[i]; \
return sum; \
}

void *generic_by_switch_and_macro(void *arr, int size, enum numeric_type type) {
    int i;
    switch (type) {
        case INT: GET_SUM(int)
        case DOUBLE: GET_SUM(double)
        default:
            return 2;//todo:
    }
}

int main() {
    int intArr[] = {2, 3, 4};
    double doubleArr[] = {0.2, 0.3, 0.4};


    int sum1 = specific_int(intArr, 3);
    printf("sum (specific int) %d\n", sum1);

    int *sum2 = (int *) generic_by_switch(intArr, 3, INT);
    printf("sum (generic_by_switch,int) %d\n", *sum2);
    free(sum2);

    double *sum3 = (double *) generic_by_switch(doubleArr, 3, DOUBLE);
    printf("sum (generic_by_switch,double) %f\n", *sum3);
    free(sum3);


    int *sum4 = (int *) generic_by_switch_and_macro(intArr, 3, INT);
    printf("sum (generic_by_switch_and_macro,int) %d\n", *sum4);
    free(sum4);


    double *sum5 = (double *) generic_by_switch_and_macro(doubleArr, 3, DOUBLE);
    printf("sum (generic_by_switch_and_macro,double) %f\n", *sum5);
    free(sum5);


    int num1 = 1;
    int num2 = 3;
    int num3 = num1 && num2;
    printf("num3 is %d\n", num3);

    return 0;
}