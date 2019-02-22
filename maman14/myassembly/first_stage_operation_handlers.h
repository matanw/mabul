
#ifndef FIRST_STAGE_OPERATION_HANDLERS_H
#define FIRST_STAGE_OPERATION_HANDLERS_H

#include "data_structures.h"

/* update the ProgramInformation structure from line of operation (mov, inc, etc)*/
void handle_operation(char *token, char *line, int *index, ProgramInformation *program_information);


#endif
