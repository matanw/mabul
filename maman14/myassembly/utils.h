#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>


/* open a file with filename and extension in specific mode*/
FILE *open_file(char *filename, char *extension, char *mode);

char *get_copy_of_string(char *str);


char get_last_char(char *string);

void delete_last_char(char *string);

#endif
