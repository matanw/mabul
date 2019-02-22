#ifndef INCLUDE_COMMAND_PARSE_H
#define INCLUDE_COMMAND_PARSE_H


/*function get pointer to char array (input) and offset (index)
 * and read next token from the offset, and increment the offset to next token.
 * the token will be in output.
 * Return value is 1 if we read a token, 0 if we arrive to and of string.
 * For example, is input is "mov @r1,@r2" and index is 0, function will return 1,
 * output will br "mov" and index will be 3*/
int get_next_token(char *input, int *index, char *output);


/*if token is register name, returns 1 , and put its number in output.
otherwise, returns 0*/
int parse_register(char *token, int *output);

/*if token is number, returns 1 , and put its number in output.
otherwise, returns 0*/
int parse_number(char *token, int *output);

/*if token is legal label, returns 1,otherwise returns 0*/
int is_legal_label(char *token);

/* if next char is expected_char, returns 1 and move forward *index, otherwise, returns 0*/
int expect_next_char(char *input, int *index, char expected_char);


/*function get pointer to char array (input) and offset (index)
 * and try to read 2 tokens to output1, output2, separated by comma
 * Return value is 1 in case of success , otherwise returns 0*/
int read_two_arguments(char *input, int *index, char *output1, char *output2);


/*function get pointer to char array (input) and offset (index)
 * and try to string, that wrapped in quotes.
 * In case of success, function return 1 and the read string will be in output,
 * otherwise function  returns 0 , and in output can be unexpected result*/
int get_string(char *input, int *index, char *output);

#endif
