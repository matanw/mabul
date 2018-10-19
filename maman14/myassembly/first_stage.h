#ifndef INCLUDE_FIRST_STAGE_H
#define INCLUDE_FIRST_STAGE_H

#include "list.h"
#include <stdio.h>

enum OPERATION {
    Mov = 0, Cmp = 1, Add = 2, Sub = 3, Not = 4, Clr = 5, Lea = 6, Inc = 7, Dec = 8,
    Jmp = 9, Bne = 10, Red = 11, Prn = 12, Jsr = 13, Rst = 14, Stop = 15, Unknown = 16
};
typedef enum OPERATION operation;


enum ADDRESSING_METHOD {
    ImmediateAddressing = 1, DirectAddressing = 3, RegisterAddressing = 5
};
typedef enum ADDRESSING_METHOD addressing_method;

struct command_line {
    int bits;
    int original_line_number;
    char *label;
};
typedef struct command_line CommandLine;

struct argument_details {
    addressing_method ad_method;
    int num;
    char *label;
};
typedef struct argument_details ArgumentDetails;

struct label_data {
    char *label;
    int code_address;
};
typedef struct label_data LabelData;

struct first_stage_data {
    List *command_lines;
    List *data_lines;
    List *label_datas;
    int original_line_number;
    int command_code_address;
    int data_code_address;
    int is_in_error;
};
typedef struct first_stage_data FirstStageData;

struct first_stage_output {
    List *command_lines;
    List *label_datas;
};
typedef struct first_stage_output FirstStageOutput;


FirstStageOutput *do_first_stage_for_file(FILE *file);

void do_first_stage_for_line(char *line, FirstStageData *first_stage_data);

void handle_label(char *token, char *line, int *index, FirstStageData *first_stage_data);

void handle_operation(char *token, char *line, int *index, FirstStageData *first_stage_data);

void handle_operation_with_2_arguments(operation op, char *line, int *index, FirstStageData *first_stage_data);


CommandLine *get_command_line(int bits, int original_line_number, char *label);

int compare_label_data_to_string(LabelData *label_data, char *label);

operation get_operation(char *op);

int fill_argument_details(char *token, ArgumentDetails *argument_details);

char *get_string_copy(char *str);

LabelData *get_label_data(char *label, int code_address);

int get_command_bits(operation op, ArgumentDetails *source_argument_details,
                     ArgumentDetails *target_argument_details, int are);

int get_two_registers_bits(ArgumentDetails *source_argument_details,
                           ArgumentDetails *target_argument_details);

int get_argument_bits(ArgumentDetails *argument_details,
                      int is_source);

int line_is_comment(char *line, int *index);

#endif
