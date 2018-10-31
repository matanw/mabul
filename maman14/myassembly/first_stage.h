#ifndef INCLUDE_FIRST_STAGE_H
#define INCLUDE_FIRST_STAGE_H

#include "list.h"
#include <stdio.h>

typedef enum OPERATION {
    Mov = 0, Cmp = 1, Add = 2, Sub = 3, Not = 4, Clr = 5, Lea = 6, Inc = 7, Dec = 8,
    Jmp = 9, Bne = 10, Red = 11, Prn = 12, Jsr = 13, Rst = 14, Stop = 15, Unknown = 16
} operation;
typedef enum SECTION_TYPE {
    Command = 1, Data = 2, None = 0
} section_type;

typedef enum ADDRESSING_METHOD {
    ImmediateAddressing = 1, DirectAddressing = 3, RegisterAddressing = 5
} addressing_method;

typedef struct command_line {
    int bits;
    int source_line_number;
    char *label;
} CommandLine;

typedef struct argument_details {
    addressing_method ad_method;
    int num;
    char *label;
} ArgumentDetails;

typedef struct label_data {
    char *label;
    int code_address;
    section_type section_type;
} LabelData;

typedef struct shared_label {
    char *label;
    int source_line_number;
} SharedLabel;
typedef struct program_information {
    List *command_lines;
    List *data_lines;
    List *label_datas;
    List *entries;
    List *external;
    int source_line_number;
    int is_in_error;
} ProgramInformation;

ProgramInformation *do_first_stage_for_file(FILE *file);

void do_first_stage_for_line(char *line, ProgramInformation *program_information);

void handle_label(char *label, section_type section_type, int old_command_lines_count, int old_data_lines_count,
                  ProgramInformation *program_information);

void
handle_shared_label(char *line, char *place_to_token, int *index, List *list, ProgramInformation *program_information);

void handle_numbers(char *place_to_token, char *line, int *index, ProgramInformation *program_information);

void handle_string(char *place_to_token, char *line, int *index, ProgramInformation *program_information);

void handle_operation(char *token, char *line, int *index, ProgramInformation *program_information);

int get_arguments_number(operation op);

void handle_operation_with_2_arguments(operation op, char *line, int *index, ProgramInformation *program_information);


void handle_operation_with_1_argument(operation op, char *line, int *index, ProgramInformation *program_information);


void handle_operation_without_arguments(operation op, char *line, int *index, ProgramInformation *program_information);

CommandLine *get_command_line(int bits, int source_line_number, char *label);

SharedLabel *get_shared_label(char *label, int source_line_number);

int *get_copy_of_int(int num);

int compare_label_data_to_string(LabelData *label_data, char *label);

operation get_operation(char *op);

int fill_argument_details(char *token, ArgumentDetails *argument_details);

char *get_string_copy(char *str);

LabelData *get_label_data(char *label, int code_address, section_type section_type);

int get_command_bits(operation op, ArgumentDetails *source_argument_details,
                     ArgumentDetails *target_argument_details, int are);

int get_two_registers_bits(ArgumentDetails *source_argument_details,
                           ArgumentDetails *target_argument_details);

int get_argument_bits(ArgumentDetails *argument_details,
                      int is_source);

int line_is_comment(char *line, int *index);


#endif
