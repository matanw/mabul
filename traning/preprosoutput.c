
int main() {
    int arr[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    int i;
    int number_with_most_zeros = arr[0];
    int nums_of_zeros;


    int a = 1;
    int b;
    {
        int j;
        b = 0;
        for (j = 0; j < 8 * sizeof(int); j++) { if (((a >> j) & 1) == 0)b++; }
    }
    printf("result is %d\n", b);

    for (i = 1; i < 8; i++) {
        {
            int count1;
            int conut2;
            {
                int j;
                count1 = 0;
                for (j = 0; j < 8 * sizeof(int); j++) { if (((number_with_most_zeros >> j) & 1) == 0)count1++; }
            }
            {
                int j;
                count2 = 0;
                for (j = 0; j < 8 * sizeof(int); j++) { if (((arr[i] >> j) & 1) == 0)count2++; }
            }
            if (count1 >= count2) {
                number_with_most_zeros = number_with_most_zeros;
                nums_of_zeros = count1;
            } else {
                number_with_most_zeros = arr[i];
                nums_of_zeros = count2;
            }
        };
    }
    printf("the num with most zero is %d, it has %d zeros", number_with_most_zeros, nums_of_zeros);
}
