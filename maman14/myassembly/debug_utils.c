#include "constants.h"
#include "debug_utils.h"
#include "bits_operations.h"


void print_command_line(CommandLine *commandLine) {
    printf("%d:", commandLine->original_line_number);
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
    printf("%d\n", *num);
}

void print_shared_label(SharedLabel *shared_label) {
    printf("%s, line %d\n", shared_label->label, shared_label->original_code_address);
}

void print_first_stage_data(FirstStageData *first_stage_data) {
    printf("command_lines:\n");
    for_each(first_stage_data->command_lines, (void (*)(void *)) print_command_line);
    printf("labels :\n");
    for_each(first_stage_data->label_datas, (void (*)(void *)) print_label_data);
    printf("data_lines:\n");
    for_each(first_stage_data->data_lines, (void (*)(void *)) print_num);
    printf("entries:\n");
    for_each(first_stage_data->entries, (void (*)(void *)) print_shared_label);
    printf("externals:\n");
    for_each(first_stage_data->external, (void (*)(void *)) print_shared_label);
}



