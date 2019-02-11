#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

#include "data_structures.h"

/* print an error massage, and mark the program_information as error*/
void handle_error(ProgramInformation *program_information, int source_line_number, const char *format, ...);

#endif
