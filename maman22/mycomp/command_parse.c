#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "command_parse.h"
#include "common.h"

int is_end_char(char ch) {
    return (ch == '\0' || ch == '\n' || ch == EOF);
}

int is_whitespace(char ch) {
    return (ch == ' ' || ch == '\t');
}

int is_comma(char ch) {
    return (ch == ',');
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
    while (!(is_comma(input[*index]) || is_end_char(input[*index]) || is_whitespace(input[*index]))) {
        *output = input[*index];
        (*index)++;
        output++;
    }
    *output = '\0';
    return 1;
}

int read_comma(char *input, int *index, char *place_to_token) {
    if (!get_next_token(input, index, place_to_token)) {
        printf("expected comma, but found end of string");
        return 0;
    }
    if (strcmp(",", place_to_token)!=0) {
        printf("expected comma, but found '%s'", place_to_token);
        return 0;
    }
    return 1;
}

int read_var_name(char *input, int *index, char *place_to_token, char *var) {
    char c;
    if (!get_next_token(input, index, place_to_token)) {
        printf("expected var name, but found end of string");
        return 0;
    }
    if (strlen(place_to_token) != 1) {
        printf("var name '%s' is more that 1 character", place_to_token);
        return 0;
    }
    c = place_to_token[0];
    if (c < 'A' || c >= 'A' + NUM_OF_COMPLEXEX) {
        printf("var name '%c' is not in range 'A'..'%c'", c, 'A' + NUM_OF_COMPLEXEX - 1);
        return 0;
    }
    *var = c;
    return 1;
}

int read_number(char *input, int *index, char *place_to_token, float *var) {
    float num;
    char *ptr;
    if (!get_next_token(input, index, place_to_token)) {
        printf("expected number, but found end of string");
        return 0;
    }
    num = strtof(place_to_token, &ptr);
    //todo:errno
    if (ptr == place_to_token || *ptr != '\0') {
        printf("'%s' is not a valid number", place_to_token);
        return 0;
    }
    *var = num;
    return 1;
}

int read_end_of_string(char *input, int *index, char *place_to_token) {
    if (get_next_token(input, index, place_to_token)) {
        printf("expected end of string, but found '%s'", place_to_token);
        return 0;
    }
    return 1;
}

int read_in_format(char *input, int *index, char *place_to_token,char *format,...) {
    va_list arguments;
    va_start ( arguments, format );
    while(*format){
        if (*format=='c'){
            char* c=va_arg ( arguments, char * );
            if(!read_var_name(input,index,place_to_token,c)){
                va_end ( arguments );
                return 0;
            }
        }
        else if (*format=='f'){
            float * f=va_arg ( arguments, float * );
            if(!read_number(input,index,place_to_token,f)){
                va_end ( arguments );
                return 0;
            }
        }
        else if (*format==','){
            if(!read_comma(input,index,place_to_token)){
                va_end ( arguments );
                return 0;
            }
        }
        format++;
    }
    if(!read_end_of_string(input,index,place_to_token)){
        va_end ( arguments );
        return 0;
    }
    return 1;
}
