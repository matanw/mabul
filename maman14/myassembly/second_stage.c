#include "second_stage.h"
#include "debug_utils.h"
#include "bits_operations.h"
#include "constants.h"
#include "utils.h"
#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void do_second_stage_for_file(ProgramInformation *program_information) {
    /*todo:assertions*/
    for_each_with_aside_var_B(program_information->command_lines, (void (*)(void *, void *, int)) fill_command_line,
                              program_information);
    printf("fill line ended\n");
    for_each_with_aside_var(program_information->entries, (void (*)(void *, void *)) fill_entry,
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

int get_real_code_address(LabelData *label_data, ProgramInformation *program_information) {
    if (label_data->section_type == Command) {
        return INITIAL_CODE_ADDRESS + label_data->code_address;
    } else {/*section_type is Data*/
        return INITIAL_CODE_ADDRESS + label_data->code_address + program_information->command_lines->count;
    }
}

ExternalRecord *get_external_record(char *label,
                                    int code_address) {/*todo: maybe crate *ctor.c*/
    ExternalRecord *external_record = (ExternalRecord *) malloc(sizeof(ExternalRecord));
    external_record->label = label;
    external_record->code_address = code_address;
    return external_record;
}

void fill_command_line(CommandLine *command_line, ProgramInformation *program_information, int index) {
    LabelData *label_data;
    if (command_line->label == NULL) {
        fill_are(command_line, Absolute);
    } else if ((label_data = search(program_information->label_datas, command_line->label,
                                    (int (*)(void *, void *)) compare_label_data_to_string2))) {
        int real_code_address;
        real_code_address = get_real_code_address(label_data, program_information);
        printf("code address %d\n", real_code_address);
        fill_address(command_line, real_code_address);
        fill_are(command_line, Relocatable);
        free(command_line->label);
        command_line->label = NULL;
    } else if (search(program_information->external, command_line->label,
                      (int (*)(void *, void *)) strcmp)) {
        int real_code_address;
        real_code_address = INITIAL_CODE_ADDRESS + index;
        add(program_information->external_records, get_external_record
                (get_string_copy(command_line->label), real_code_address));
        fill_are(command_line, External);
        free(command_line->label);
        command_line->label = NULL;
    } else {
/*todo:error*/
    }
}

void fill_entry(Entry *entry, ProgramInformation *program_information) {
    LabelData *label_data;
    if ((label_data = search(program_information->label_datas, entry->label,
                             (int (*)(void *, void *)) compare_label_data_to_string2))) {
        int real_code_address;
        real_code_address = get_real_code_address(label_data, program_information);
        entry->code_address = real_code_address;
    } else {
        printf("entry not exits");
        program_information->is_in_error = 1;
    }
}


