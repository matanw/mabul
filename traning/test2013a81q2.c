#define COUNT_ZERO(num, result, type) {\
int j;\
result=0;\
for(j=0;j<8*sizeof(type);j++){\
if(((num>>j) &1)==0)\
result++;\
}\
}
#define FIND_ZEROS(num1, num2, result_num, result_count, type) {\
int count1;\
int count2;\
COUNT_ZERO(num1,count1,type)\
COUNT_ZERO(num2,count2,type)\
if (count1>=count2)\
{\
result_num=num1;\
result_count=count1;\
}\
else \
{\
result_num=num2;\
result_count=count2;\
}\
}

#include <stdio.h>

/*כאן נכנס המקרו מסעיף א*/
int main() {
    int arr[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    int i;
    int number_with_most_zeros = arr[0];
    int nums_of_zeros;


    int a = 1;
    int b;
    COUNT_ZERO(a, b, int)
    printf("result is %d\n", b);

    for (i = 1; i < 8; i++) {
        FIND_ZEROS(number_with_most_zeros, arr[i], number_with_most_zeros, nums_of_zeros, int);
    }
    printf("the num with most zero is %d, it has %d zeros", number_with_most_zeros, nums_of_zeros);
}
