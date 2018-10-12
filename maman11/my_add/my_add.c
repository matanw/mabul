#include<stdio.h>

#define BINARY_DIGIT_AMOUNT 8 * sizeof(int)

/* get the digit-num -th (count starts from zero) digit of num in binary base*/
int get_digit(unsigned int num, int digit_num) {
    return (num >> digit_num) & 1;
}

/* set the digit-num -th (count starts from zero) digit of num in binary base to one*/
void set_digit_one(unsigned int *num, int digit_num) {
    *num |= (1 << digit_num);
}


void print_binary(unsigned int num) {
    int i;
    for (i= BINARY_DIGIT_AMOUNT; i >= 0; i--)
        printf("%d", get_digit(num, i));
}


unsigned int inner_my_add(unsigned int a, unsigned int b) {
    unsigned int sum = 0;
    int addition_to_next = 0;
    int digit_num;
    for (digit_num = 0; digit_num < BINARY_DIGIT_AMOUNT; digit_num++) {
        int digit_sum = get_digit(a, digit_num) + get_digit(b, digit_num) + addition_to_next;
        if (digit_sum == 0) {
            addition_to_next = 0;
        } else if (digit_sum == 1) {
            set_digit_one(&sum, digit_num);
            addition_to_next = 0;
        } else if (digit_sum == 2) {
            addition_to_next = 1;
        } else /*digit_sum is 3*/
        {
            set_digit_one(&sum, digit_num);
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
    scanf("%u %u", &a, &b);
    printf("numbers are %u ,%u\n", a, b);
    printf("sum is :%u", my_add(a, b));

    return 0;
}
