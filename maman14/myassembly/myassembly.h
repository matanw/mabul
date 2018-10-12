
#include<stdio.h>

int process_file_1(FILE* file);
CommandLine* get_commandLine(int bits,int original_line_number,char* label);
void print_command(CommandLine* commandLine);
void print_label_data(LabelData *label_data);
int fill_argument_details(char *token,ArgumentDetails* argument_details);

int get_command_bits(operation op, ArgumentDetails *source_argument_details,
                     ArgumentDetails *target_argument_details, int are);
int get_two_registers_bits(ArgumentDetails *source_argument_details,
                           ArgumentDetails *target_argument_details);
int get_argument_bits(ArgumentDetails *argument_details,
                                int is_source);

char* get_string_copy(char* str);

LabelData* get_label_data(char *label, int code_address);
int compare_label_data_to_string(LabelData *label_data, char *label) ;
