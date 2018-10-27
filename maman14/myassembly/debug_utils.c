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

void print_command_lines(List *command_lines) {
    for_each(command_lines, (void (*)(void *)) print_command_line);
}

void print_label_datas(List *label_datas) {
    for_each(label_datas, (void (*)(void *)) print_label_data);
}

void print_nums(List *nums) {
    for_each(nums, (void (*)(void *)) print_num);
}



