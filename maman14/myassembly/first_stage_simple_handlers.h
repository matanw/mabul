

#ifndef FIRST_STAGE_SIMPLE_HANDLERS_H
#define FIRST_STAGE_SIMPLE_HANDLERS_H

#include "data_structures.h"

/* update the ProgramInformation structure from line of  type ".entry"*/
void handle_entry(char *line, char *place_to_token, int *index, ProgramInformation *program_information);


/* update the ProgramInformation structure from line of  type ".extern"*/
void handle_extern(char *line, char *place_to_token, int *index, ProgramInformation *program_information);

/* update the ProgramInformation structure from line with label (in addition to handle the logic of line)*/
void handle_label(char *label, section_type section_type, int old_command_lines_count, int old_data_lines_count,
                  ProgramInformation *program_information);

/* update the ProgramInformation structure from line of  type ".data"*/
void handle_numbers(char *place_to_token, char *line, int *index, ProgramInformation *program_information);

/* update the ProgramInformation structure from line of  type ".string"*/
void handle_string(char *place_to_token, char *line, int *index, ProgramInformation *program_information);

#endif
