
#include "utils.h"
#include "constants.h"
#include "stdlib.h"
#include <string.h>

/* open a file with filename and extension in specific mode*/
FILE *open_file(char *filename, char *extension, char *mode) {
    char extended_file_name[MAX_FILE_NAME_LENGTH];
    strcpy(extended_file_name, filename);
    strcat(extended_file_name, extension);
    return fopen(extended_file_name, mode);
}

char *get_copy_of_string(char *str) {
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


int *get_copy_of_int(int num) {
    int *res = malloc(sizeof(int));
    *res = num;
    return res;
}
