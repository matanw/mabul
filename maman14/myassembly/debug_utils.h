#ifndef INCLUDE_DEBUG_UTILS_H
#define INCLUDE_DEBUG_UTILS_H

#include "first_stage.h"
#include "list.h"

void print_command_line(CommandLine *commandLine);

void print_label_data(LabelData *label_data);

void print_num(int *num);

void print_shared_label(SharedLabel *shared_label);

void print_first_stage_data(FirstStageData *first_stage_data);

#endif
