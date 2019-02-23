#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include "command_parse.h"

int is_end_char(char ch) {
    return (ch == '\0' || ch == '\n' || ch == EOF);
}

int is_whitespace(char ch) {
    return (ch == ' ' || ch == '\t');
}

int is_comma(char ch) {
    return (ch == ',');
}

int is_colon(char ch) {
    return (ch == ':');
}

/*function get pointer to char array (input) and offset (index)
 * and read next token from the offset, and increment the offset to next token.
 * the token will be in output.
 * Return value is 1 if we read a token, 0 if we arrive to and of string.
 * For example, is input is "mov @r1,@r2" and index is 0, function will return 1,
 * output will br "mov" and index will be 3*/
int get_next_token(char *input, int *index, char *output) {
    while (is_whitespace(input[*index])) {
        (*index)++;
    }
    if (is_end_char(input[*index])) {
        return 0;
    }
    if (is_comma(input[*index])) {
        strcpy(output, ",");
        (*index)++;
        return 1;
    }
    while (!(is_comma(input[*index]) || is_end_char(input[*index]) || is_whitespace(input[*index]))) {
        *output = input[*index];
        (*index)++;
        output++;
        if (is_colon(*(output - 1))) {  /*if we found a colon, we return the string with colon in end*/
            break;
        }
    }
    *output = '\0';
    return 1;
}


/*if token is register name, returns 1 , and put its number in output.
otherwise, returns 0*/
int parse_register(char *token, int *output) {
    if (token[0] != '@' || token[1] != 'r' || token[3] != '\0') {
        return 0;
    }
    if (token[2] < '1' || token[2] > '8') {
        return 0;
    }
    *output = token[2] - '0';
    return 1;
}


/*if token is number, returns 1 , and put its number in output.
otherwise, returns 0*/
int parse_number(char *token, int *output) {
    char *ptr;
    long size_to_module = 1 << 13;
    long num = strtol(token, &ptr, 10);
    if (ptr == token || *ptr != '\0') {
        return 0;
    }
    *output = num % size_to_module;
    return 1;
}


/*if token is legal label, returns 1,otherwise returns 0*/
int is_legal_label(char *token) {
    if (strcmp("mov", token) == 0 || strcmp("cmp", token) == 0 || strcmp("add", token) == 0 ||
        strcmp("sub", token) == 0 || strcmp("not", token) == 0 || strcmp("clr", token) == 0 ||
        strcmp("lea", token) == 0 || strcmp("inc", token) == 0 || strcmp("dec", token) == 0 ||
        strcmp("jmp", token) == 0 || strcmp("bne", token) == 0 || strcmp("red", token) == 0 ||
        strcmp("prn", token) == 0 || strcmp("jsr", token) == 0 || strcmp("rts", token) == 0 ||
        strcmp("stop", token) == 0 || strcmp("data", token) == 0 || strcmp("string", token) == 0 ||
        strcmp("extern", token) == 0 || strcmp("entry", token) == 0) {
        return 0;
    }
    if (!isalpha(*(token++))) {
        return 0;
    }
    while (*token) {
        if (!isalnum(*(token++))) {
            return 0;
        }
    }
    return 1;
}


/* if next char is expected_char, returns 1 and move forward *index, otherwise, returns 0*/
int expect_next_char(char *input, int *index, char expected_char) {
    while (is_whitespace(input[*index])) {
        (*index)++;
    }
    if (input[*index] == expected_char) {
        (*index)++;
        return 1;
    }
    return 0;
}


/*function get pointer to char array (input) and offset (index)
 * and try to read 2 tokens to output1, output2, separated by comma
 * Return value is 1 in case of success , otherwise returns 0*/
int read_two_arguments(char *input, int *index, char *output1, char *output2) {
    if (!get_next_token(input, index, output1)) {
        return 0;
    }
    if (!expect_next_char(input, index, ',')) {
        return 0;
    }
    if (!get_next_token(input, index, output2)) {
        return 0;
    }
    return 1;
}


/*function get pointer to char array (input) and offset (index)
 * and try to string, that wrapped in quotes.
 * In case of success, function return 1 and the read string will be in output,
 * otherwise function  returns 0 , and in output can be unexpected result*/
int get_string(char *input, int *index, char *output) {
    while (is_whitespace(input[*index])) {
        (*index)++;
    }
    if (input[*index] != '"') {
        return 0;
    }
    while (1) {
        (*index)++;
        if (input[*index] == '"') {
            (*index)++;
            *output = '\0';
            return 1;
        }
        if (is_end_char(input[*index])) {
            return 0;
        }
        *output = input[*index];
        output++;
    }
}

