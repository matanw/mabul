#ifndef SECOND_STAGE_H
#define SECOND_STAGE_H

#include "data_structures.h"

typedef enum ARE {
    Absolute = 0, Relocatable = 2, External = 1
} are;

void do_second_stage_for_file(ProgramInformation *program_information);

void fill_label(CommandLine *command_line, ProgramInformation *program_information);

#endif
