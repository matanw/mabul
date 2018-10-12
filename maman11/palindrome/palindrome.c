#include<stdio.h>
#include <string.h>
#define MAX_LENGTH 80
int palindrome(char s[]) {
    int len = strlen(s);
    char *first = s;
    char *last = s + len - 1;
    if (*last == '\n') {
        last--;
    }
    while (first < last) {
        while (*first == ' ' || *first == '\t')
            first++;
        while (*last == ' ' || *last == '\t')
            last--;
        if (*first != *last)
            return 0;
        first++;
        last--;
    }
    return 1;
}

int main(void) {
    char s[MAX_LENGTH];
    if (fgets(s, sizeof s, stdin)) {/*todo:remove if?*/
        printf("input is \"%s\"\n", s);
        printf("result is %d", palindrome(s));
    }
    return 0;
}
