
#include "debug_utils.h"
#include "bits_operations.h"
#include <stdio.h>
#include "list.h"
void print_command_line(CommandLine *commandLine) {
    printf("%d:", commandLine->source_line_number);
    print_bits(commandLine->bits);
    if (commandLine->label != NULL) {
        printf(" , label %s", commandLine->label);
    }
    printf("\n");
}

void print_label_data(LabelData *label_data) {
    printf("label:%s, %d\n", label_data->label, label_data->code_address);
}

void print_num(int *num) {
    printf("%d  - ", *num);
    print_bits(*num);
    printf("\n");
}

void print_entry(Entry *entry) {
    printf("%s, line %d\n", entry->label, entry->source_line_number);
}

void print_string(char *string) {
    printf("%s\n", string);
}

void print_external_record(ExternalRecord *external_record) {
    printf("%s - %d\n", external_record->label, external_record->code_address);
}

/*print the print_program_information struct, for debugging*/
void print_program_information(ProgramInformation *program_information) {
    printf("file name:%s\n",program_information->file_name);
    printf("command_lines:\n");
    poor_for_each(program_information->command_lines, (void (*)(void *)) print_command_line);
    printf("labels :\n");
    poor_for_each(program_information->label_datas, (void (*)(void *)) print_label_data);
    printf("data_lines:\n");
    poor_for_each(program_information->data_lines, (void (*)(void *)) print_num);
    printf("entries:\n");
    poor_for_each(program_information->entries, (void (*)(void *)) print_entry);
    printf("externals:\n");
    poor_for_each(program_information->external, (void (*)(void *)) print_string);
    printf("external records:\n");
    poor_for_each(program_information->external_records, (void (*)(void *)) print_external_record);
}



