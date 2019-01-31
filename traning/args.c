

#include<stdarg.h>
#include<stdio.h>

void my_sum(int *, int, ...);

int wrap_my_sum(int num_args, ...);

void my_printf(char *fmt, ...) {

    va_list argptr;
    va_start(argptr, fmt);
    printf(fmt, argptr);
    va_end(argptr);
}

void Error(const char *format, ...) {
    char dest[1024 * 16];
    va_list argptr;
    va_start(argptr, format);
    vsprintf(dest, format, argptr);
    va_end(argptr);
    int line = 4;
    fprintf(stderr, "line %d : %s\n", line, dest);
}

void Error2(const char *format, ...) {
    int line = 4;
    va_list argptr;
    va_start(argptr, format);
    fprintf(stderr, "line %d : ", line);
    vfprintf(stderr, format, argptr);
    va_end(argptr);
}

int main(void) {
    int i = 1;
    Error2("one is %d\n", i);

    printf("my_sum of 1, 2 and 3 = %d\n", wrap_my_sum(3, 1, 2, 3));

    return 0;
}

void my_sum(int *res, int num_args, ...) {
    int val = 0;
    int num;
    va_list ap;
    int i;

    va_start(ap, num_args);
    for (i = 0; i < num_args; i++) {
        num = va_arg(ap, int);
        printf("num is %d\n", num);
        num += val;
    }
    va_end(ap);

    *res = val;
}

int wrap_my_sum(int num_args, ...) {

    va_list argptr;
    int res;
    va_start(argptr, num_args);
    my_sum(&res, num_args, argptr);
    va_end(argptr);
    return res;
}

