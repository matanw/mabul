
#include "io_utils.h"
#include "constants.h"
#include <string.h>

FILE *fopen_with_extension(char *filename, char *extension, char *mode) {
    char extended_file_name[MAX_FILE_NAME_LENGTH];
    strcpy(extended_file_name, filename);
    strcat(extended_file_name, extension);
    return fopen(extended_file_name, mode);
}
