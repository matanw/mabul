int get_next_token(char* input,int* index, char* output);

int read_comma(char *input, int *index, char *place_to_token);
int read_var_name(char *input, int *index, char *place_to_token, char *var);
int read_number(char *input, int *index, char *place_to_token, float *var);
int read_end_of_string(char *input, int *index, char *place_to_token);

int read_in_format(char *input, int *index, char *place_to_token,char *format,...);
