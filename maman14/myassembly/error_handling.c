

#include "error_handling.h"
#include <stdio.h>
#include <stdarg.h>

void handle_error(ProgramInformation *program_information, int source_line_number, const char *format, ...) {
    va_list arg_ptr;

    if (!program_information->is_in_error) {/* there are not errors yet, this is the first in file*/

        fprintf(stderr, "******* Errors in file %s *******\n", program_information->file_name);
    }
    fprintf(stderr, "line %d: ", source_line_number);

    va_start(arg_ptr, format);
    vfprintf(stderr, format, arg_ptr);
    va_end(arg_ptr);

    fprintf(stderr, "\n");
    program_information->is_in_error = 1;
}
