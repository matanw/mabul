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

int is_special_char(char ch) {
    return (ch == ':' || ch == ',');
}

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
    while (!(is_special_char(input[*index]) || is_end_char(input[*index]) || is_whitespace(input[*index]))) {
        *output = input[*index];
        (*index)++;
        output++;
    }
    *output = '\0';
    return 1;
}

int read_comma(char *input, int *index, char *place_to_token) {/*todo:remove prints?*/
    if (!get_next_token(input, index, place_to_token)) {
        printf("expected comma, but found end of string");
        return 0;
    }
    if (strcmp(",", place_to_token) != 0) {
        printf("expected comma, but found '%s'", place_to_token);
        return 0;
    }
    return 1;
}

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

int parse_number(char *token, int *output) {
    char *ptr;
    long num = strtol(token, &ptr, 10);
    /*todo: check that long in range*/
    if (ptr == token || *ptr != '\0') {
        return 0;
    }
    *output = num;
    return 1;
}

int is_legal_label(char *token) {
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

int expect_next_char(char *input, int *index, char expected_char) {/*todo:rename*/
    while (is_whitespace(input[*index])) {
        (*index)++;
    }
    if (input[*index] == expected_char) {
        (*index)++;
        return 1;
    }
    return 0;
}

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


int get_string(char *input, int *index, char *token) {
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
            *token = '\0';
            return 1;
        }
        if (is_end_char(input[*index])) {
            return 0;
        }
        *token = input[*index];
        token++;
    }
}
