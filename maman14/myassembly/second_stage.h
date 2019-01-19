#ifndef SECOND_STAGE_H
#define SECOND_STAGE_H

#include "data_structures.h"

typedef enum ARE {
    Absolute = 0, Relocatable = 2, External = 1
} are;

void do_second_stage_for_file(ProgramInformation *program_information);

void fill_command_line(CommandLine *command_line, ProgramInformation *program_information, int index);

void fill_entry(Entry *entry, ProgramInformation *program_information);


void assert_duplication_labels(LabelData *label_data1, LabelData *label_data2,
                               ProgramInformation *program_information);

void assert_label_and_external_not_equals(LabelData *label_data, char *external,
                                          ProgramInformation *program_information);

#endif
