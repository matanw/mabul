#include <stdio.h>
#include <stdlib.h>

// A normal function with an int parameter
// and void return type 
void fun(int a) {
    printf("Value of a is %d\n", a);
}

void fun2(int a, int b) {
    printf("Value of a is %d , b is %d\n", a, b);
}

int *fun3(int a, int b) {
    int *p = (int *) malloc(sizeof(int));
    *p = a + b;
    return p;
}

int main() {
    // fun_ptr is a pointer to function fun()  
    void (*fun_ptr)(int) = &fun;

    /* The above line is equivalent of following two 
       void (*fun_ptr)(int); 
       fun_ptr = &fun;  
    */

    // Invoking fun() using fun_ptr 
    (*fun_ptr)(10);

    void (*fun_ptr2)(int, int) = &fun2;
    fun_ptr2(5, 5);
    int *(*fun_ptr3)(int, int) = &fun3;
    printf("%d\n", *fun_ptr3(3, 4));


    void (fun_ptr4)(int) = fun;

    return 0;
} 