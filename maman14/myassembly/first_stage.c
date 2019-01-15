
#include "first_stage.h"
#include "constants.h"
#include "list.h"
#include "command_parse.h"
#include "debug_utils.h"
#include "bits_operations.h"
#include "utils.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>


#include <stdarg.h>

/*todo: move to modoule*/
void print_error(ProgramInformation *program_information, const char *format, ...) {
    va_list arg_ptr;
    va_start(arg_ptr, format);
    if (!program_information->is_in_error) {/* there are not errors yet, this is the first in file*/

        fprintf(stderr, "******* Errors in file %s *******\n", program_information->file_name);
    }
    fprintf(stderr, "line %d: ", program_information->source_line_number);
    vfprintf(stderr, format, arg_ptr);
    fprintf(stderr, "\n");
    va_end(arg_ptr);
}

ProgramInformation *do_first_stage_for_file(char *file_name, int is_debug_mode) {
    char line[MAX_LINE_LENGTH];
    ProgramInformation *program_information;
    FILE *file = fopen_with_extension(file_name, SOURCE_FILE_EXTENSION, "r");
    if (file == NULL) {
        return NULL;
    }
    program_information = init_program_information(file_name, is_debug_mode);

    while (fgets(line, sizeof line, file)) {
        program_information->source_line_number++;
        do_first_stage_for_line(line, program_information);
    }
    fclose(file);
    if (program_information->is_debug_mode) {
        print_error(program_information, "***after first stage:***\n");
        print_program_information(program_information);
    }
    return program_information;
}

ProgramInformation *init_program_information(char *file_name, int is_debug_mode) {
    ProgramInformation *program_information = malloc(sizeof(ProgramInformation));
    program_information->is_debug_mode = is_debug_mode;
    program_information->command_lines = init_list();
    program_information->data_lines = init_list();
    program_information->label_datas = init_list();
    program_information->entries = init_list();
    program_information->external = init_list();
    program_information->external_records = init_list();
    program_information->source_line_number = 0;
    program_information->is_in_error = 0;
    program_information->file_name = get_string_copy(file_name);
    return program_information;

}

int line_is_comment(char *line, int *index) {
    return expect_next_char(line, index, ';');
}

void do_first_stage_for_line(char *line, ProgramInformation *program_information) {
    char token[MAX_LINE_LENGTH];
    int old_command_lines_count = program_information->command_lines->count;
    int old_data_lines_count = program_information->data_lines->count;
    char *label = NULL;
    section_type section_type;
    int index = 0;
    if (line_is_comment(line, &index)) {
        return;
    }
    if (!get_next_token(line, &index, token))/*blank line*/
    {
        return;
    }
    if (get_last_char(token) == ':') {
        label = get_string_copy(token);
        if (!get_next_token(line, &index, token))/*blank line*/
        {
            print_error(program_information, "empty label is illegal");
            program_information->is_in_error = 1;
            return;
        }
    }

    if (strcmp(token, NUMBERS_PREFIX) == 0) {
        handle_numbers(token, line, &index, program_information);
        section_type = Data;
    } else if (strcmp(token, STRING_PREFIX) == 0) {
        handle_string(token, line, &index, program_information);
        section_type = Data;
    } else if (strcmp(token, ENTRY_PREFIX) == 0) {
        handle_entry(line, token, &index, program_information);
        section_type = None;
    } else if (strcmp(token, EXTERNAL_PREFIX) == 0) {
        handle_extern(line, token, &index, program_information);
        section_type = None;
    } else {
        handle_operation(token, line, &index, program_information);
        section_type = Command;
    }
    handle_label(label, section_type, old_command_lines_count, old_data_lines_count, program_information);
}

void handle_entry(char *line, char *place_to_token, int *index, ProgramInformation *program_information) {
    if (!get_next_token(line, index, place_to_token)) {
        print_error(program_information, "expected label name, found end of string");
        program_information->is_in_error = 1;
        return;
    }
    if (!is_legal_label(place_to_token)) {
        print_error(program_information, "'%s' is not a legal label", place_to_token);
        program_information->is_in_error = 1;
        return;
    }
    add(program_information->entries,
        get_entry(get_string_copy(place_to_token), program_information->source_line_number));
    if (get_next_token(line, index, place_to_token)) {
        print_error(program_information, "expected end of string but found '%s'", place_to_token);
        program_information->is_in_error = 1;
        return;
    }
}

void handle_extern(char *line, char *place_to_token, int *index, ProgramInformation *program_information) {
    if (!get_next_token(line, index, place_to_token)) {
        print_error(program_information, "expected label name, found end of string");
        program_information->is_in_error = 1;
        return;
    }
    if (!is_legal_label(place_to_token)) {
        print_error(program_information, "'%s' is not a legal label", place_to_token);
        program_information->is_in_error = 1;
        return;
    }
    add(program_information->external, get_string_copy(place_to_token));
    if (get_next_token(line, index, place_to_token)) {
        print_error(program_information, "expected end of string but found '%s'", place_to_token);
        program_information->is_in_error = 1;
        return;
    }
}

void handle_label(char *label, section_type section_type, int old_command_lines_count, int old_data_lines_count,
                  ProgramInformation *program_information) {
    LabelData *label_data;
    int count_to_insert;
    if (label == NULL) {
        return;
    }
    delete_last_char(label);
    if (!is_legal_label(label)) {
        print_error(program_information, "'%s' is not a legal label", label);
        free(label);
        program_information->is_in_error = 1;
        return;
    }
    if (section_type == None) {
        print_error(program_information, "warn: label with .extern or .entity has no meaning");
        /*no mark error*/
        free(label);
        return;
    }
    if (search(program_information->label_datas, label, (int (*)(void *, void *)) compare_label_data_to_string) !=
        NULL) {
        print_error(program_information, "'%s' already exists", label);
        free(label);
        program_information->is_in_error = 1;
        return;
    }
    count_to_insert = (section_type == Command ? old_command_lines_count : old_data_lines_count);
    label_data = get_label_data(label, count_to_insert, section_type);
    add(program_information->label_datas, label_data);
}

void handle_numbers(char *place_to_token, char *line, int *index, ProgramInformation *program_information) {
    int num;
    if (!get_next_token(line, index, place_to_token)) {
        print_error(program_information, "expected number, but found end of string");
        program_information->is_in_error = 1;
        return;
    }
    if (!parse_number(place_to_token, &num)) {
        print_error(program_information, "'%s' is not a number", place_to_token);
        program_information->is_in_error = 1;
        return;
    }
    add(program_information->data_lines, get_copy_of_int(num));
    while (1) {
        if (!get_next_token(line, index, place_to_token)) {
            /*end to string, valid state*/
            return;
        }
        if (strcmp(place_to_token, ",") != 0) {
            print_error(program_information, "expected comma but '%s' was found", place_to_token);
            program_information->is_in_error = 1;
            return;
        }

        if (!get_next_token(line, index, place_to_token)) {
            print_error(program_information, "expected number, but found end of string");
            program_information->is_in_error = 1;
            return;
        }
        if (!parse_number(place_to_token, &num)) {
            print_error(program_information, "'%s' is not a number", place_to_token);
            program_information->is_in_error = 1;
            return;
        }
        add(program_information->data_lines, get_copy_of_int(num));
    }
}

void handle_string(char *place_to_token, char *line, int *index, ProgramInformation *program_information) {
    char *c;
    if (!get_string(line, index, place_to_token)) {
        print_error(program_information, "error while reading string");
        program_information->is_in_error = 1;
        return;
    }
    if (get_next_token(line, index, place_to_token)) {
        print_error(program_information, "expected end of line after string, but found '%s'", place_to_token);
        program_information->is_in_error = 1;
        return;
    }
    c = place_to_token;
    while (*c) {
        add(program_information->data_lines, get_copy_of_int((int) *c));
        c++;
    }
    add(program_information->data_lines, get_copy_of_int(0));
}

void handle_operation(char *token, char *line, int *index, ProgramInformation *program_information) {
    operation op;
    int arguments_num;
    op = get_operation(token);
    if (op == Unknown) {
        print_error(program_information, "command not found :%s", token);
        program_information->is_in_error = 1;
        return;
    }
    arguments_num = get_arguments_number(op);
    switch (arguments_num) {
        case 2:
            handle_operation_with_2_arguments(op, line, index, program_information);
            break;
        case 1:
            handle_operation_with_1_argument(op, line, index, program_information);
            break;
        default:
            handle_operation_without_arguments(op, line, index, program_information);
    }
}

int get_arguments_number(operation op) {
    switch (op) {
        case Mov:
        case Cmp:
        case Add:
        case Sub:
        case Lea:
            return 2;
        case Not:
        case Clr:
        case Inc:
        case Dec:
        case Jmp:
        case Bne:
        case Red:
        case Prn:
        case Jsr:
            return 1;
        default:
            return 0;
    }
}

void handle_operation_with_2_arguments(operation op, char *line, int *index, ProgramInformation *program_information) {
    char source_argument[MAX_LINE_LENGTH];
    char target_argument[MAX_LINE_LENGTH];
    int command_bits;
    ArgumentDetails source_argument_details, target_argument_details;
    if (!read_two_arguments(line, index, source_argument, target_argument)) {
        print_error(program_information, "except to 2 arguments but there aren't");
        program_information->is_in_error = 1;
        return;
    }
    if (!fill_argument_details(source_argument, &source_argument_details)) {
        print_error(program_information, "'%s' is not register, number, or valid label name", source_argument);
        program_information->is_in_error = 1;
        return;
    }
    if (!fill_argument_details(target_argument, &target_argument_details)) {
        print_error(program_information, "'%s' is not register, number, or valid label name", target_argument);
        program_information->is_in_error = 1;
        return;
    }

    if (get_next_token(line, index, source_argument)) {/*todo:another var?*/
        print_error(program_information, "excepted end of line but found %s", source_argument);
        program_information->is_in_error = 1;
        return;
    }
    command_bits = get_command_bits(op, &source_argument_details, &target_argument_details);
    add(program_information->command_lines,
        get_command_line(command_bits, program_information->source_line_number, NULL));
    if (source_argument_details.ad_method == RegisterAddressing &&
        target_argument_details.ad_method == RegisterAddressing) {
        int registers_bits = get_two_registers_bits(&source_argument_details,
                                                    &target_argument_details);
        add(program_information->command_lines,
            get_command_line(registers_bits, program_information->source_line_number, NULL));
    } else {
        int source_bits, target_bits;
        source_bits = get_argument_bits(&source_argument_details, 1);
        add(program_information->command_lines,
            get_command_line(source_bits, program_information->source_line_number,
                             source_argument_details.label));
        target_bits = get_argument_bits(&target_argument_details, 0);
        add(program_information->command_lines,
            get_command_line(target_bits, program_information->source_line_number,
                             target_argument_details.label));
    }
}

void handle_operation_with_1_argument(operation op, char *line, int *index, ProgramInformation *program_information) {
    char argument[MAX_LINE_LENGTH];
    int command_bits, argument_bits;
    ArgumentDetails argument_details;
    if (!get_next_token(line, index, argument)) {
        print_error(program_information, "except to argument but there isn't");
        program_information->is_in_error = 1;
        return;
    }
    if (!fill_argument_details(argument, &argument_details)) {
        print_error(program_information, "'%s' is not register, number, or valid label name", argument);
        program_information->is_in_error = 1;
        return;
    }
    if (get_next_token(line, index, argument)) {/*todo:another var?*/
        print_error(program_information, "excepted end of line but found %s", argument);
        program_information->is_in_error = 1;
        return;
    }
    command_bits = get_command_bits(op, NULL, &argument_details);
    add(program_information->command_lines,
        get_command_line(command_bits, program_information->source_line_number, NULL));
    argument_bits = get_argument_bits(&argument_details, 0);
    add(program_information->command_lines,
        get_command_line(argument_bits, program_information->source_line_number,
                         argument_details.label));
}

void handle_operation_without_arguments(operation op, char *line, int *index, ProgramInformation *program_information) {
    char text_in_end_of_line[MAX_LINE_LENGTH];
    int command_bits;

    if (get_next_token(line, index, text_in_end_of_line)) {
        print_error(program_information, "excepted end of line but found %s", text_in_end_of_line);
        program_information->is_in_error = 1;
        return;
    }
    command_bits = get_command_bits(op, NULL, NULL);
    add(program_information->command_lines,
        get_command_line(command_bits, program_information->source_line_number, NULL));
}

CommandLine *get_command_line(int bits, int source_line_number, char *label) {
    CommandLine *command_line = (CommandLine *) malloc(sizeof(CommandLine));
    command_line->bits = bits;
    command_line->source_line_number = source_line_number;
    command_line->label = label;
    return command_line;
}

int compare_label_data_to_string(LabelData *label_data, char *label) {
    return strcmp(label_data->label, label);
}

operation get_operation(char *op) {
    if (strcmp("mov", op) == 0) {
        return Mov;
    }
    if (strcmp("cmp", op) == 0) {
        return Cmp;
    }
    if (strcmp("add", op) == 0) {
        return Add;
    }
    if (strcmp("sub", op) == 0) {
        return Sub;
    }
    if (strcmp("not", op) == 0) {
        return Not;
    }
    if (strcmp("clr", op) == 0) {
        return Clr;
    }
    if (strcmp("lea", op) == 0) {
        return Lea;
    }
    if (strcmp("inc", op) == 0) {
        return Inc;
    }
    if (strcmp("dec", op) == 0) {
        return Dec;
    }
    if (strcmp("jmp", op) == 0) {
        return Jmp;
    }
    if (strcmp("bne", op) == 0) {
        return Bne;
    }
    if (strcmp("red", op) == 0) {
        return Red;
    }
    if (strcmp("prn", op) == 0) {
        return Prn;
    }
    if (strcmp("jsr", op) == 0) {
        return Jsr;
    }
    if (strcmp("rst", op) == 0) {
        return Rst;
    }
    if (strcmp("stop", op) == 0) {
        return Stop;
    }
    return Unknown;
}

int fill_argument_details(char *token, ArgumentDetails *argument_details) {
    if (parse_register(token, &(argument_details->num))) {
        argument_details->ad_method = RegisterAddressing;
        argument_details->label = NULL;
        return 1;
    }
    if (parse_number(token, &(argument_details->num))) {
        argument_details->ad_method = ImmediateAddressing;
        argument_details->label = NULL;
        return 1;
    }
    if (is_legal_label(token)) {
        argument_details->ad_method = DirectAddressing;
        argument_details->num = 0;
        argument_details->label = get_string_copy(token);
        return 1;
    }
    return 0;
}

LabelData *get_label_data(char *label, int code_address, section_type section_type) {
    LabelData *label_data;
    label_data = malloc(sizeof(LabelData));
    label_data->label = label;
    label_data->code_address = code_address;
    label_data->section_type = section_type;
    return label_data;
}

Entry *get_entry(char *label, int source_line_number) {
    Entry *entry;
    entry = malloc(sizeof(entry));
    entry->label = label;
    entry->source_line_number = source_line_number;
    return entry;
}

int *get_copy_of_int(int num) {
    int *res = malloc(sizeof(int));
    *res = num;
    return res;
}

int get_command_bits(operation op, ArgumentDetails *source_argument_details,
                     ArgumentDetails *target_argument_details) {
    int bits = 0;
    put_bits(&bits, op, OP_CODE_POSITION);
    if (source_argument_details != NULL) {
        put_bits(&bits, source_argument_details
                ->ad_method, SOURCE_ADDRESSING_POSITION);
        put_bits(&bits, op, OP_CODE_POSITION);
    }
    if (target_argument_details != NULL) {
        put_bits(&bits, target_argument_details
                ->ad_method, TARGET_ADDRESSING_POSITION);
    }
    return bits;
}

int get_two_registers_bits(ArgumentDetails *source_argument_details,
                           ArgumentDetails *target_argument_details) {
    int bits = 0;
    put_bits(&bits, source_argument_details
            ->num, SOURCE_REGISTER_POSITION);
    put_bits(&bits, target_argument_details
            ->num, TARGET_REGISTER_POSITION);
    return bits;
}

int get_argument_bits(ArgumentDetails *argument_details,
                      int is_source) {
    int bits = 0;
    if (argument_details->ad_method == ImmediateAddressing) {
        put_bits(&bits, argument_details
                ->num, NUMBER_POSITION);
    } else if (argument_details->ad_method == RegisterAddressing) {
        put_bits(&bits, argument_details
                ->num, (is_source ? SOURCE_REGISTER_POSITION : TARGET_REGISTER_POSITION));
    }
    return bits;
}

