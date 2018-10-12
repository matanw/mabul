
#include<stdio.h>

int process_file_1(FILE* file);
CommandLine* get_commandLine(int bits,int original_line_number);
void print_command(CommandLine* commandLine);
int fill_argument_details(char *token,ArgumentDetails* argument_details);

int get_command_bits(operation op, ArgumentDetails *source_argument_details,
                     ArgumentDetails *target_argument_details, int are);
int get_two_registers_bits(ArgumentDetails *source_argument_details,
                           ArgumentDetails *target_argument_details);
int get_argument_registers_bits(ArgumentDetails *argument_details,
                                int is_source);
