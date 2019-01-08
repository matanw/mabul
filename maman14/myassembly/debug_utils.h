#ifndef INCLUDE_DEBUG_UTILS_H
#define INCLUDE_DEBUG_UTILS_H

#include "data_structures.h"
#include "list.h"

void print_command_line(CommandLine *commandLine);

void print_label_data(LabelData *label_data);

void print_num(int *num);

void print_entry(Entry *entry);

void print_program_information(ProgramInformation *program_information);

#endif
