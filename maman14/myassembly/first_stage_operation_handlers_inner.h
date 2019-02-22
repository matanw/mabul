
#ifndef FIRST_STAGE_OPERATION_HANDLERS_INNER_H
#define FIRST_STAGE_OPERATION_HANDLERS_INNER_H

#include "data_structures.h"

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

/* constrains of addressing method*/
typedef struct addressing_methods_constraints {
    unsigned int is_immediate_addressing_valid:1;
    unsigned int is_direct_addressing_valid:1;
    unsigned int is_register_addressing_valid:1;
} AddressingMethodsConstraints;


/* get the arguments number for operation*/
int get_arguments_number(operation op);

/* create an AddressingMethodsConstraints*/
AddressingMethodsConstraints
*get_addressing_methods_constraints(unsigned int is_immediate_addressing_valid, unsigned int is_direct_addressing_valid,
                                    unsigned int is_register_addressing_valid);

/* get the constraints of argument for operation that accepts one argument*/
AddressingMethodsConstraints *get_addressing_methods_constraints_for_one_argument_operation(operation op);


/* get the constraints of source argument for operation that accepts two arguments*/
AddressingMethodsConstraints *get_addressing_methods_constraints_for_source_argument(operation op);


/* get the constraints of target argument for operation that accepts two arguments*/
AddressingMethodsConstraints *get_addressing_methods_constraints_for_target_argument(operation op);


/* update the ProgramInformation structure from line of operation that accept two arguments*/
void handle_operation_with_2_arguments(operation op, char *line, int *index, ProgramInformation *program_information);


/* put the operation name  in operation_name*/
void put_operation_name(operation op, char *operation_name);


/* put the addressing method name  in addressing_method_name*/
void put_addressing_method_name(addressing_method addressing_method, char *addressing_method_name);

/* assert the the addressing_method is legal to operation*/
void assert_addressing_method(addressing_method addressing_method,
                              AddressingMethodsConstraints *addressing_methods_constraints,
                              ProgramInformation *program_information,
                              operation op,
                              const char *argument_description);

/* update the ProgramInformation structure from line of operation that accept one argument*/
void handle_operation_with_1_argument(operation op, char *line,
                                      int *index, ProgramInformation *program_information);

/* update the ProgramInformation structure from line of operation that doesn't accept arguments*/
void handle_operation_without_arguments(operation op, char *line, int *index,
                                        ProgramInformation *program_information);

/**create a commandLine structure*/
CommandLine *get_command_line(int bits, int source_line_number, char *label);

/*get an opertaion by string*/
operation get_operation(char *op);

/*fill the ArgumentDetails structure from token*/
int fill_argument_details(char *token, ArgumentDetails *argument_details);

/* get bits of commands, as int number  (bits of the command itself)*/
int get_command_bits(operation op, ArgumentDetails *source_argument_details,
                     ArgumentDetails *target_argument_details);

/* get bits of 2 registers in one word*/
int get_two_registers_bits(ArgumentDetails *source_argument_details,
                           ArgumentDetails *target_argument_details);


/* get bits of ab argument*/
int get_argument_bits(ArgumentDetails *argument_details,
                      int is_source);


#endif
