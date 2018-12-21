
/*function get pointer to char array (input) and offset (index)
 * and read next token from the offset, and increment the offset to next token.
 * the token will be in output.
 * Return value is 1 if we read a token, 0 if we arrive to and of string.
 * For example, is input is "add_comp A, B" and index is 0, function will return 1,
 * output will br "add_comp" and index will be 8*/
int get_next_token(char *input, int *index, char *output);


/*function get pointer to char array (input) and offset (index)
 * and read try to read a comma from input, starting from index.
 *If it success, it returns 1, otherwise, it returns 0 and print error message to user.
 * Function get place to token for internal use
 * Side effect: increment the index to next token index,
 * as I explained in get_next_token documentation*/
int read_comma(char *input, int *index, char *place_to_token);

/*function get pointer to char array (input) and offset (index)
 * and read try to read a variable name (one of letters 'A'-'F')
 * from input, starting from index.
 *If it success, it returns 1 and locate the var name in *var,
 *otherwise, it returns 0 and print error message to user.
 * Function get place to token for internal use
 * Side effect: increment the index to next token index,
 * as I explained in get_next_token documentation*/
int read_var_name(char *input, int *index, char *place_to_token, char *var);


/*function get pointer to char array (input) and offset (index)
 * and read try to read a number from input, starting from index.
 *If it success, it returns 1, and puts the number in *var
 * the otherwise, it returns 0 and print error message to user.
 * Function get place to token for internal use
 * Side effect: increment the index to next token index,
 * as I explained in get_next_token documentation*/
int read_number(char *input, int *index, char *place_to_token, float *var);

/*function get pointer to char array (input) and offset (index)
* and validate string ends starting from index.
*If it success, it returns 1, otherwise, it returns 0 and print error message to user.
* Function get place to token for internal use
* Side effect: increment the index to next token index,
* as I explained in get_next_token documentation*/
int read_end_of_string(char *input, int *index, char *place_to_token);

/*function get pointer to char array (input) and offset (index)
 * , format string and list of pointers.
 * function try to read from input, starting from index, what the format reqired.
 * Format can contain 3 characters:
 * c - represents variable name
 * f - represents float number
 * , - represent comma
 * For example, read_in_format(input,index,place_to_token,"c,f,f",&c,&f1,&f2)
 * try to read var name to c, and after it float nuber to f1 and float number to f2,
 * separated by commas.
 * If it success, it returns 1, otherwise, it returns 0 and print error message to user.
 * Function get place to token for internal use
 * Side effect: increment the index to next token index,
 * as I explained in get_next_token documentation*/

int read_in_format(char *input, int *index, char *place_to_token, char *format, ...);
