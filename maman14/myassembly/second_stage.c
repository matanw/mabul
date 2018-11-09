#include "second_stage.h"
#include "debug_utils.h"
#include "bits_operations.h"
#include "constants.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void do_second_stage_for_file(ProgramInformation *program_information) {
    /*todo:assertions*/
    for_each_with_aside_var(program_information->command_lines, (void (*)(void *, void *)) fill_label,
                            program_information);
    print_program_information(program_information);
}

void fill_are(CommandLine *command_line, are are) {
    put_bits(&command_line->bits, are, ARE_POSITION);
}

void fill_address(CommandLine *command_line, int address) {
    put_bits(&command_line->bits, address, LABEL_POSITION);
}

int compare_label_data_to_string2(LabelData *label_data, char *label) {/*todo: same function exists in first_stage*/
    return strcmp(label_data->label, label);
}

int get_code_address(LabelData *label_data, ProgramInformation *program_information) {
    if (label_data->section_type == Command) {
        return INITIAL_CODE_ADDRESS + label_data->code_address;
    } else {/*section_type is Data*/
        return INITIAL_CODE_ADDRESS + label_data->code_address + program_information->command_lines->count;
    }
}

void fill_label(CommandLine *command_line, ProgramInformation *program_information) {
    LabelData *label_data;
    if (command_line->label == NULL) {
        fill_are(command_line, Absolute);
        return;
    }
    if ((label_data = search(program_information->label_datas, command_line->label,
                             (int (*)(void *, void *)) compare_label_data_to_string2))) {
        int code_address;
        free(command_line->label);
        command_line->label = NULL;
        code_address = get_code_address(label_data, program_information);
        printf("code address %d\n", code_address);
        fill_address(command_line, code_address);
        fill_are(command_line, Relocatable);
        return;
    }

}


