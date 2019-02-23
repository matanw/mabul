
#include <stdlib.h>
#include <string.h>
#include "first_stage_operation_handlers.h"
#include "first_stage_operation_handlers_inner.h"
#include "error_handling.h"
#include "constants.h"
#include "command_parse.h"
#include "utils.h"
#include "bits_operations.h"
#include "list.h"


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
    if (get_next_token(line, index, argument)) {
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

