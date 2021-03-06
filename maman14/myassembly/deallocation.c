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

void free_entry(Entry *entry) {
    if (entry->label != NULL)
        free(entry->label);
}

void free_external_record(ExternalRecord *external_record) {
    if (external_record->label != NULL)
        free(external_record->label);
}


/*safe free to program_information and all its sub structure*/
void free_program_information(ProgramInformation *program_information) {

    free_list(program_information->command_lines, (void (*)(void *))
            free_command_line_indirect);
    free_list(program_information->label_datas, (void (*)(void *))
            free_label_data_indirect);
    free_list(program_information->data_lines, (void (*)(void *))
            NULL);
    free_list(program_information->entries, (void (*)(void *))
            free_entry);
    free_list(program_information->external, (void (*)(void *))
            NULL);
    free_list(program_information->external_records, (void (*)(void *))
            free_external_record);
    free(program_information->file_name);
    free(program_information);
}
