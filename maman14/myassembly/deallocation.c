#include "deallocation.h"
#include "list.h"
#include <stdlib.h>

void free_label_data_indirect(LabelData *label_data) {
    if (label_data->label != NULL)
        free(label_data->label);
}

void free_command_line_indirect(CommandLine *command_line) {
    if (command_line->label != NULL)
        free(command_line->label);
}

void free_shared_label(SharedLabel *shared_label) {
    if (shared_label->label != NULL)
        free(shared_label->label);
}

void free_program_information(ProgramInformation *program_information) {

    free_list(program_information->command_lines, (void (*)(void *))
            free_command_line_indirect);
    free_list(program_information->label_datas, (void (*)(void *))
            free_label_data_indirect);
    free_list(program_information->data_lines, (void (*)(void *))
            NULL);
    free_list(program_information->entries, (void (*)(void *))
            free_shared_label);
    free_list(program_information->external, (void (*)(void *))
            free_shared_label);
    free(program_information);
}
