#ifndef INCLUDE_FIRST_STAGE_H
#define INCLUDE_FIRST_STAGE_H

#include "list.h"
#include "data_structures.h"
#include <stdio.h>

typedef enum OPERATION {
    Mov = 0, Cmp = 1, Add = 2, Sub = 3, Not = 4, Clr = 5, Lea = 6, Inc = 7, Dec = 8,
    Jmp = 9, Bne = 10, Red = 11, Prn = 12, Jsr = 13, Rst = 14, Stop = 15, Unknown = 16
} operation;
typedef enum ADDRESSING_METHOD {
    ImmediateAddressing = 1, DirectAddressing = 3, RegisterAddressing = 5
} addressing_method;

typedef struct argument_details {
    addressing_method ad_method;
    int num;
    char *label;
} ArgumentDetails;

typedef struct addressing_methods_constraints {
    unsigned int is_immediate_addressing_valid:1;
    unsigned int is_direct_addressing_valid:1;
    unsigned int is_register_addressing_valid:1;
} AddressingMethodsConstraints;

AddressingMethodsConstraints *get_addressing_methods_constraints
        (unsigned int is_immediate_addressing_valid, unsigned int is_direct_addressing_valid,
         unsigned int is_register_addressing_valid);

AddressingMethodsConstraints *get_addressing_methods_constraints_for_one_argument_operation(operation op);

int assert_argument_type(addressing_method addressing_method,
                         AddressingMethodsConstraints *addressing_methods_constraints,
                         ProgramInformation *program_information,
                         operation op,
                         const char *argument_description);

void put_operation_name(operation op, char *operation_name);

void put_addressing_method_name(addressing_method addressing_method, char *addressing_method_name);

ProgramInformation *do_first_stage_for_file(char *file_name, int is_debug_mode);

ProgramInformation *init_program_information(char *file_name, int is_debug_mode);

void do_first_stage_for_line(char *line, ProgramInformation *program_information);

void handle_label(char *label, section_type section_type, int old_command_lines_count, int old_data_lines_count,
                  ProgramInformation *program_information);

void handle_entry(char *line, char *place_to_token, int *index, ProgramInformation *program_information);

void handle_extern(char *line, char *place_to_token, int *index, ProgramInformation *program_information);

void handle_numbers(char *place_to_token, char *line, int *index, ProgramInformation *program_information);

void handle_string(char *place_to_token, char *line, int *index, ProgramInformation *program_information);

void handle_operation(char *token, char *line, int *index, ProgramInformation *program_information);

int get_arguments_number(operation op);

void handle_operation_with_2_arguments(operation op, char *line, int *index, ProgramInformation *program_information);


void handle_operation_with_1_argument(operation op, char *line, int *index, ProgramInformation *program_information);


void handle_operation_without_arguments(operation op, char *line, int *index, ProgramInformation *program_information);

CommandLine *get_command_line(int bits, int source_line_number, char *label);

Entry *get_entry(char *label, int source_line_number);

int *get_copy_of_int(int num);

int compare_label_data_to_string(LabelData *label_data, char *label);

operation get_operation(char *op);

int fill_argument_details(char *token, ArgumentDetails *argument_details);

LabelData *get_label_data(char *label, int code_address, section_type section_type,
                          int source_line_number);

int get_command_bits(operation op, ArgumentDetails *source_argument_details,
                     ArgumentDetails *target_argument_details);

int get_two_registers_bits(ArgumentDetails *source_argument_details,
                           ArgumentDetails *target_argument_details);

int get_argument_bits(ArgumentDetails *argument_details,
                      int is_source);

int line_is_comment(char *line, int *index);


#endif
