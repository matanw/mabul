
#include "utils.h"
#include "constants.h"
#include "stdlib.h"
#include <string.h>

FILE *fopen_with_extension(char *filename, char *extension, char *mode) {
    char extended_file_name[MAX_FILE_NAME_LENGTH];
    strcpy(extended_file_name, filename);
    strcat(extended_file_name, extension);
    return fopen(extended_file_name, mode);
}

char *get_string_copy(char *str) {
    char *res;
    res = malloc((strlen(str) + 1) * sizeof(char));
    strcpy(res, str);
    return res;
}


char get_last_char(char *string) {
    return string[strlen(string) - 1];
}

void delete_last_char(char *string) {
    string[strlen(string) - 1] = '\0';
}
