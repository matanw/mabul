#ifndef INCLUDE_COMMAND_PARSE_H
#define INCLUDE_COMMAND_PARSE_H


int get_next_token(char *input, int *index, char *output);

int read_comma(char *input, int *index, char *place_to_token);

/*if token is register name, returns 1 , and put its number in output.
otherwise, returns 0*/
int parse_register(char *token, int *output);

/*if token is number, returns 1 , and put its number in output.
otherwise, returns 0*/
int parse_number(char *token, int *output);

/*if token is legal label, returns 1,oterise returns 0*/
int is_legal_label(char *token);

/* if next char is expected_char and move forward *index, otherwise, returns 0*/
int expect_next_char(char *input, int *index, char expected_char);/*todo:rename*/


int read_two_arguments(char *input, int *index, char *output1, char *output2);

#endif
