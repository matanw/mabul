#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

FILE *fopen_with_extension(char *filename, char *extension, char *mode);

char *get_string_copy(char *str);


char get_last_char(char *string);

void delete_last_char(char *string);

#endif
