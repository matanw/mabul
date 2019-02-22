
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "first_stage.h"
#include "constants.h"
#include "list.h"
#include "command_parse.h"
#include "debug_utils.h"
#include "bits_operations.h"
#include "utils.h"
#include "error_handling.h"

/*do this first stage (מעבר ראשון) on the file.
 * first stage read the file line by line and parse every line by itself,
 * without reference to other lines*/
ProgramInformation *do_first_stage_for_file(char *file_name, int is_debug_mode) {
    char line[MAX_LINE_LENGTH];
    ProgramInformation *program_information;
    FILE *file = open_file(file_name, SOURCE_FILE_EXTENSION, "r");
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
        printf("***after first stage:***\n");
        print_program_information(program_information);
    }
    return program_information;
}

/*initialize the ProgramInformation sturct*/
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
    program_information->file_name = get_copy_of_string(file_name);
    return program_information;

}
 /*return 1 if line is a comment, 0 otherwise*/
int is_line_a_comment(char *line, int *index) {
    return expect_next_char(line, index, ';');
}

/* do the first stage for a single line*/
void do_first_stage_for_line(char *line, ProgramInformation *program_information) {
    char token[MAX_LINE_LENGTH];
    int old_command_lines_count = program_information->command_lines->count;
    int old_data_lines_count = program_information->data_lines->count;
    char *label = NULL;
    section_type section_type;
    int index = 0;
    if (is_line_a_comment(line, &index)) {
        return;
    }
    if (!get_next_token(line, &index, token))/*blank line*/
    {
        return;
    }
    if (get_last_char(token) == ':') {
        label = get_copy_of_string(token);
        if (!get_next_token(line, &index, token))/*blank line*/
        {
            handle_error(program_information, program_information->source_line_number, "empty label is illegal");
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

/* update the ProgramInformation structure from line of  type ".entry"*/
void handle_entry(char *line, char *place_to_token, int *index, ProgramInformation *program_information) {
    if (!get_next_token(line, index, place_to_token)) {
        handle_error(program_information, program_information->source_line_number,
                     "expected label name, found end of string");
        return;
    }
    if (!is_legal_label(place_to_token)) {
        handle_error(program_information, program_information->source_line_number, "'%s' is not a legal label",
                     place_to_token);
        return;
    }
    add(program_information->entries,
        get_entry(get_copy_of_string(place_to_token), program_information->source_line_number));
    if (get_next_token(line, index, place_to_token)) {
        handle_error(program_information, program_information->source_line_number,
                     "expected end of string but found '%s'", place_to_token);
        return;
    }
}


/* update the ProgramInformation structure from line of  type ".extern"*/
void handle_extern(char *line, char *place_to_token, int *index, ProgramInformation *program_information) {
    if (!get_next_token(line, index, place_to_token)) {
        handle_error(program_information, program_information->source_line_number,
                     "expected label name, found end of string");
        return;
    }
    if (!is_legal_label(place_to_token)) {
        handle_error(program_information, program_information->source_line_number, "'%s' is not a legal label",
                     place_to_token);
        return;
    }
    add(program_information->external, get_copy_of_string(place_to_token));
    if (get_next_token(line, index, place_to_token)) {
        handle_error(program_information, program_information->source_line_number,
                     "expected end of string but found '%s'", place_to_token);
        program_information->is_in_error = 1;
        return;
    }
}


/* update the ProgramInformation structure from line with label (in addition to handle the logic of line)*/
void handle_label(char *label, section_type section_type, int old_command_lines_count, int old_data_lines_count,
                  ProgramInformation *program_information) {
    LabelData *label_data;
    int count_to_insert;
    if (label == NULL) {
        return;
    }
    delete_last_char(label);
    if (!is_legal_label(label)) {
        handle_error(program_information, program_information->source_line_number, "'%s' is not a legal label", label);
        free(label);
        return;
    }
    if (section_type == None) {
        free(label);
        return;
    }
    count_to_insert = (section_type == Command ? old_command_lines_count : old_data_lines_count);
    label_data = get_label_data(label, count_to_insert, section_type, program_information->source_line_number);
    add(program_information->label_datas, label_data);
}


/* update the ProgramInformation structure from line of  type ".data"*/
void handle_numbers(char *place_to_token, char *line, int *index, ProgramInformation *program_information) {
    int num;
    if (!get_next_token(line, index, place_to_token)) {
        handle_error(program_information, program_information->source_line_number,
                     "expected number, but found end of string");
        return;
    }
    if (!parse_number(place_to_token, &num)) {
        handle_error(program_information, program_information->source_line_number, "'%s' is not a number",
                     place_to_token);
        return;
    }
    add(program_information->data_lines, get_copy_of_int(num));
    while (1) {
        if (!get_next_token(line, index, place_to_token)) {
            /*end to string, valid state*/
            return;
        }
        if (strcmp(place_to_token, ",") != 0) {
            handle_error(program_information, program_information->source_line_number,
                         "expected comma but '%s' was found", place_to_token);
            program_information->is_in_error = 1;
            return;
        }

        if (!get_next_token(line, index, place_to_token)) {
            handle_error(program_information, program_information->source_line_number,
                         "expected number, but found end of string");
            program_information->is_in_error = 1;
            return;
        }
        if (!parse_number(place_to_token, &num)) {
            handle_error(program_information, program_information->source_line_number, "'%s' is not a number",
                         place_to_token);
            return;
        }
        add(program_information->data_lines, get_copy_of_int(num));
    }
}


/* update the ProgramInformation structure from line of  type ".string"*/
void handle_string(char *place_to_token, char *line, int *index, ProgramInformation *program_information) {
    char *c;
    if (!get_string(line, index, place_to_token)) {
        handle_error(program_information, program_information->source_line_number, "error while reading string");
        return;
    }
    if (get_next_token(line, index, place_to_token)) {
        handle_error(program_information, program_information->source_line_number,
                     "expected end of line after string, but found '%s'", place_to_token);
        return;
    }
    c = place_to_token;
    while (*c) {
        add(program_information->data_lines, get_copy_of_int((int) *c));
        c++;
    }
    add(program_information->data_lines, get_copy_of_int(0));
}


/* update the ProgramInformation structure from line of operation (mov, inc, etc)*/
void handle_operation(char *token, char *line, int *index, ProgramInformation *program_information) {
    operation op;
    int arguments_num;
    op = get_operation(token);
    if (op == Unknown) {
        handle_error(program_information, program_information->source_line_number, "command not found :%s", token);
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



/* get the arguments number for operation*/
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

/* create an AddressingMethodsConstraints*/
AddressingMethodsConstraints
*get_addressing_methods_constraints(unsigned int is_immediate_addressing_valid, unsigned int is_direct_addressing_valid,
                                    unsigned int is_register_addressing_valid) {
    AddressingMethodsConstraints *addressing_methods_constraints = malloc(sizeof(AddressingMethodsConstraints));
    addressing_methods_constraints->is_immediate_addressing_valid = is_immediate_addressing_valid;
    addressing_methods_constraints->is_direct_addressing_valid = is_direct_addressing_valid;
    addressing_methods_constraints->is_register_addressing_valid = is_register_addressing_valid;
    return addressing_methods_constraints;
}

/* get the constraints of argument for operation that accepts one argument*/
AddressingMethodsConstraints *get_addressing_methods_constraints_for_one_argument_operation(operation op) {
    switch (op) {
        case Not:
            return get_addressing_methods_constraints(0, 1, 1);
        case Clr:
            return get_addressing_methods_constraints(0, 1, 1);
        case Inc:
            return get_addressing_methods_constraints(0, 1, 1);
        case Dec:
            return get_addressing_methods_constraints(0, 1, 1);
        case Jmp:
            return get_addressing_methods_constraints(0, 1, 1);
        case Bne:
            return get_addressing_methods_constraints(0, 1, 1);
        case Red:
            return get_addressing_methods_constraints(0, 1, 1);
        case Prn:
            return get_addressing_methods_constraints(1, 1, 1);
        case Jsr:
            return get_addressing_methods_constraints(0, 1, 1);
        default:
            return NULL;/*this is only for avoid warning,
 * there is no scenario that it will be occurred*/
    }
}


/* get the constraints of source argument for operation that accepts two arguments*/
AddressingMethodsConstraints *get_addressing_methods_constraints_for_source_argument(operation op) {
    switch (op) {
        case Mov:
        case Cmp:
        case Add:
        case Sub:
            return get_addressing_methods_constraints(1, 1, 1);
        case Lea:
            return get_addressing_methods_constraints(0, 1, 0);
        default:
            return NULL;/*this is only for avoid warning,
 * there is no scenario that it will be occurred*/
    }
}


/* get the constraints of target argument for operation that accepts two arguments*/
AddressingMethodsConstraints *get_addressing_methods_constraints_for_target_argument(operation op) {
    switch (op) {
        case Mov:
            return get_addressing_methods_constraints(0, 1, 1);
        case Cmp:
            return get_addressing_methods_constraints(1, 1, 1);
        case Add:
        case Sub:
        case Lea:
            return get_addressing_methods_constraints(0, 1, 1);
        default:
            return NULL;/*this is only for avoid warning,
 * there is no scenario that it will be occurred*/
    }
}



/* update the ProgramInformation structure from line of operation that accept two arguments*/
void handle_operation_with_2_arguments(operation op, char *line, int *index, ProgramInformation *program_information) {
    char source_argument[MAX_LINE_LENGTH];
    char target_argument[MAX_LINE_LENGTH];
    AddressingMethodsConstraints *source_addressing_method_constraints;
    AddressingMethodsConstraints *target_addressing_method_constraints;
    int command_bits;
    ArgumentDetails source_argument_details, target_argument_details;
    if (!read_two_arguments(line, index, source_argument, target_argument)) {
        handle_error(program_information, program_information->source_line_number,
                     "except to 2 arguments but there aren't");
        return;
    }
    if (!fill_argument_details(source_argument, &source_argument_details)) {
        handle_error(program_information, program_information->source_line_number,
                     "'%s' is not register, number, or valid label name", source_argument);
        return;
    }
    if (!fill_argument_details(target_argument, &target_argument_details)) {
        handle_error(program_information, program_information->source_line_number,
                     "'%s' is not register, number, or valid label name", target_argument);
        return;
    }

    if (get_next_token(line, index, source_argument)) {/*todo:another var?*/
        handle_error(program_information, program_information->source_line_number, "excepted end of line but found %s",
                     source_argument);
        return;
    }
    source_addressing_method_constraints = get_addressing_methods_constraints_for_source_argument(op);
    assert_addressing_method(source_argument_details.ad_method, source_addressing_method_constraints,
                         program_information, op, "source argument");
    target_addressing_method_constraints = get_addressing_methods_constraints_for_target_argument(op);
    assert_addressing_method(target_argument_details.ad_method, target_addressing_method_constraints,
                         program_information, op, "target argument");
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


/* put the operation name  in operation_name*/
void put_operation_name(operation op, char *operation_name) {
    switch (op) {
        case Mov :
            strcpy(operation_name, "mov");
            break;
        case Cmp :
            strcpy(operation_name, "cmp");
            break;
        case Add  :
            strcpy(operation_name, "add");
            break;
        case Sub  :
            strcpy(operation_name, "sub");
            break;
        case Not :
            strcpy(operation_name, "not");
            break;
        case Clr  :
            strcpy(operation_name, "clr");
            break;
        case Lea :
            strcpy(operation_name, "lea");
            break;
        case Inc  :
            strcpy(operation_name, "inc");
            break;
        case Dec  :
            strcpy(operation_name, "dec");
            break;
        case Jmp  :
            strcpy(operation_name, "jmp");
            break;
        case Bne  :
            strcpy(operation_name, "bne");
            break;
        case Red  :
            strcpy(operation_name, "red");
            break;
        case Prn  :
            strcpy(operation_name, "prn");
            break;
        case Jsr :
            strcpy(operation_name, "jsr");
            break;
        case Rst  :
            strcpy(operation_name, "res");
            break;
        case Stop  :
            strcpy(operation_name, "stop");
            break;
        case Unknown:
            strcpy(operation_name, "--");/*only to avoid warning*/
    }

}


/* put the addressing method name  in addressing_method_name*/
void put_addressing_method_name(addressing_method addressing_method, char *addressing_method_name) {
    switch (addressing_method) {
        case ImmediateAddressing  :
            strcpy(addressing_method_name, "immediate");
            break;
        case DirectAddressing :
            strcpy(addressing_method_name, "direct");
            break;
        case RegisterAddressing :
            strcpy(addressing_method_name, "register");
    }
}

/* assert the the addressing_method is legal to operation*/
void assert_addressing_method(addressing_method addressing_method,
                          AddressingMethodsConstraints *addressing_methods_constraints,
                          ProgramInformation *program_information,
                          operation op,
                          const char *argument_description) {
    if ((addressing_method == ImmediateAddressing
         && !addressing_methods_constraints->is_immediate_addressing_valid) ||
        (addressing_method == DirectAddressing
         && !addressing_methods_constraints->is_direct_addressing_valid) ||
        (addressing_method == RegisterAddressing
         && !addressing_methods_constraints->is_register_addressing_valid)) {
        char operation_name[OPERATION_NAME_LENGTH];
        char addressing_method_name[ADDRESSING_METHOD_NAME_LENGTH];
        put_operation_name(op, operation_name);
        put_addressing_method_name(addressing_method, addressing_method_name);
        handle_error(program_information, program_information->source_line_number,
                     "%s of operation '%s' cannot be in  %s addressing method",
                     argument_description, operation_name, addressing_method_name);
    }
}

/* update the ProgramInformation structure from line of operation that accept one argument*/
void handle_operation_with_1_argument(operation op, char *line,
        int *index, ProgramInformation *program_information) {
    char argument[MAX_LINE_LENGTH];
    int command_bits, argument_bits;
    ArgumentDetails argument_details;
    AddressingMethodsConstraints *addressing_methods_constraints;
    if (!get_next_token(line, index, argument)) {
        handle_error(program_information, program_information->source_line_number,
                     "except to argument but there isn't");
        return;
    }
    if (!fill_argument_details(argument, &argument_details)) {
        handle_error(program_information, program_information->source_line_number,
                     "'%s' is not register, number, or valid label name", argument);
        return;
    }
    if (get_next_token(line, index, argument)) {/*todo:another var?*/
        handle_error(program_information, program_information->source_line_number,
                     "excepted end of line but found %s",
                     argument);
        return;
    }
    addressing_methods_constraints =
            get_addressing_methods_constraints_for_one_argument_operation(op);

    assert_addressing_method(argument_details.ad_method,
                         addressing_methods_constraints, program_information, op, "argument");
    command_bits = get_command_bits(op, NULL, &argument_details);
    add(program_information->command_lines,
        get_command_line(command_bits, program_information->source_line_number, NULL));
    argument_bits = get_argument_bits(&argument_details, 0);
    add(program_information->command_lines,
        get_command_line(argument_bits, program_information->source_line_number,
                         argument_details.label));
}

/* update the ProgramInformation structure from line of operation that doesn't accept arguments*/
void handle_operation_without_arguments(operation op, char *line, int *index,
        ProgramInformation *program_information) {
    char text_in_end_of_line[MAX_LINE_LENGTH];
    int command_bits;

    if (get_next_token(line, index, text_in_end_of_line)) {
        handle_error(program_information, program_information->source_line_number,
                     "excepted end of line but found %s",
                     text_in_end_of_line);
        return;
    }
    command_bits = get_command_bits(op, NULL, NULL);
    add(program_information->command_lines,
        get_command_line(command_bits, program_information->source_line_number, NULL));
}

/**create a commandLine structure*/
CommandLine *get_command_line(int bits, int source_line_number, char *label) {
    CommandLine *command_line = (CommandLine *) malloc(sizeof(CommandLine));
    command_line->bits = bits;
    command_line->source_line_number = source_line_number;
    command_line->label = label;
    return command_line;
}

/*get an opertaion by string*/
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

/*fill the ArgumentDetails structure from token*/
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
        argument_details->label = get_copy_of_string(token);
        return 1;
    }
    return 0;
}

/*crate a LabelData structure*/
LabelData *get_label_data(char *label, int code_address, section_type section_type,
                          int source_line_number) {
    LabelData *label_data;
    label_data = malloc(sizeof(LabelData));
    label_data->label = label;
    label_data->code_address = code_address;
    label_data->section_type = section_type;
    label_data->source_line_number = source_line_number;
    return label_data;
}


/*crate a Entry structure*/
Entry *get_entry(char *label, int source_line_number) {
    Entry *entry;
    entry = malloc(sizeof(entry));
    entry->label = label;
    entry->source_line_number = source_line_number;
    return entry;
}

/* get bits of commands, as int number  (bits of the command itself)*/
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


/* get bits of 2 registers in one word*/
int get_two_registers_bits(ArgumentDetails *source_argument_details,
                           ArgumentDetails *target_argument_details) {
    int bits = 0;
    put_bits(&bits, source_argument_details
            ->num, SOURCE_REGISTER_POSITION);
    put_bits(&bits, target_argument_details
            ->num, TARGET_REGISTER_POSITION);
    return bits;
}


/* get bits of ab argument*/
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

