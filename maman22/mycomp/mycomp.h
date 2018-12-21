
/*print a complex number to stdout*/
void print_complex(Complex num);

/*print a real number to stdout*/
void print_real(float num);

/*resolves the problem of print "-0"*/
float fix_minus_zero(float num);


/*init a complex numbers*/
void init_complex(Complex *num);

/*init the array of complex numbers*/
void init_array(Complex *nums);

/* Get pointer to array of complex number, line that rea from stdin,
 * and place to token
 * (to not allocate and fre every running)
 * Function executes the command in line, print errors if needed,
 * and return 1 if it is need to stop the execution, 0 otherwise*/
int execute_command(Complex *nums, char *line, char *token);
