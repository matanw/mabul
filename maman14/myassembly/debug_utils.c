#include "constants.h"
#include "debug_utils.h"
#include "bits_operations.h"
#include <stdio.h>

void print_command_line(CommandLine *commandLine) {
    printf("%d:", commandLine->source_line_number);
    print_bits_int(commandLine->bits);
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
    print_bits_int(*num);
    printf("\n");
}

void print_shared_label(SharedLabel *shared_label) {
    printf("%s, line %d\n", shared_label->label, shared_label->source_line_number);
}

void print_program_information(ProgramInformation *program_information) {
    printf("command_lines:\n");
    for_each(program_information->command_lines, (void (*)(void *)) print_command_line);
    printf("labels :\n");
    for_each(program_information->label_datas, (void (*)(void *)) print_label_data);
    printf("data_lines:\n");
    for_each(program_information->data_lines, (void (*)(void *)) print_num);
    printf("entries:\n");
    for_each(program_information->entries, (void (*)(void *)) print_shared_label);
    printf("externals:\n");
    for_each(program_information->external, (void (*)(void *)) print_shared_label);
}



