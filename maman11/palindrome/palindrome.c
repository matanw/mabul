#include<stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 82 /*80 chars of string + 1 for \n +1 for \0 */

/*return 1 if string is palindrome, otherwise 0*/
int palindrome(char s[]) {
    int len = strlen(s);
    char *first = s;
    char *last = s + len - 1;
    while (first < last) {
        while (isspace(*first))
            first++;
        while (isspace(*last))
            last--;
        if (*first != *last) {
            return 0;
        }
        first++;
        last--;
    }
    return 1;
}

/* if string ends with \n, delete the '\n' character*/
void delete_last_new_line(char *s) {
    while (*s) {
        s++;
    }
    if (*(s - 1) == '\n') {
        *(s - 1) = '\0';
    }
}

int main(void) {
    char s[MAX_LENGTH];
    printf("Please insert a line:");
    if (!fgets(s, sizeof s, stdin)) {
        printf("Error while reading string\n");
        return 1;
    }
    delete_last_new_line(s);
    printf("input is \"%s\"\n", s);
    printf("result is %d\n", palindrome(s));
    return 0;
}
