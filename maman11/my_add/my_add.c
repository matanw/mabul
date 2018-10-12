#include<stdio.h>


/*todo: camelCase or not*/
#define BINARY_DIGIT_AMOUNT 5 /*todo:choose number*/


int get_digit(unsigned int num, int digitNum) {
    return (num >> digitNum) & 1;
}

void set_digit_one(unsigned int *num, int digitNum) {
    *num |= (1 << digitNum);
}


void print_binary(unsigned int num) {
    int i;
    for (i= BINARY_DIGIT_AMOUNT; i >= 0; i--)
        printf("%d", get_digit(num, i));
}


unsigned int inner_my_add(unsigned int a, unsigned int b) {
    unsigned int sum = 0;
    int addition_to_next = 0;
    int digitNum;
    for (digitNum = 0; digitNum < BINARY_DIGIT_AMOUNT; digitNum++) {
        int digit_sum = get_digit(a, digitNum) + get_digit(b, digitNum) + addition_to_next;
        if (digit_sum == 0) {
            addition_to_next = 0;
        } else if (digit_sum == 1) {
            set_digit_one(&sum, digitNum);
            addition_to_next = 0;
        } else if (digit_sum == 2) {
            addition_to_next = 1;
        } else /*digit_sum is 3*/
        {
            set_digit_one(&sum, digitNum);
            addition_to_next = 1;
        }
    }
    return sum;
}

unsigned int my_add(unsigned int a, unsigned int b) {
    unsigned int sum;
    printf("first number in binary base is ");
    print_binary(a);
    printf("\n");
    printf("second number in binary base is ");
    print_binary(b);
    printf("\n");
    sum = inner_my_add(a, b);
    printf("sum in binary base is ");
    print_binary(sum);
    printf("\n");
    return sum;
}

int main() {
    unsigned int a, b;
    printf("Please enter two number: \n");
    scanf("%d %d", &a, &b);
    printf("numbers are %d ,%d\n", a, b);
    printf("sum is :%d", my_add(a, b));

    return 0;
}
