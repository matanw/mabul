#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "complex.h"
#include "mycomp.h"
#include "command_parse.h"
#include "common.h"


/*print a complex number to stdout*/
void print_complex(Complex num) {
    printf("%.6g+(%.6g)i\n", fix_minus_zero(num.real_part), fix_minus_zero(num.imaginary_part));
}

/*print a real number to stdout*/
void print_real(double num) {
    printf("%.6g\n", fix_minus_zero(num));
}

/*resolves the problem of print "-0"*/
double fix_minus_zero(double num) {
    return (num == 0 ? 0 : num);
}


/*init a complex numbers*/
void init_complex(Complex *num) {
    num->real_part = 0;
    num->imaginary_part = 0;
}

/*init the array of complex numbers*/
void init_array(Complex *nums) {
    int i;
    for (i = 0; i <= NUM_OF_COMPLEXEX; i++) {
        init_complex(&nums[i]);
    }
}

/*get pointer to array of complex number, line that rea from stdin,
 * and place to token
 * (to not allocate and fre every running)
 * Function executes the command in line, print errors if needed,
 * and return 1 if it is need to stop the execution, 0 otherwise*/
int execute_command(Complex *nums, char *line, char *token) {
    int index;
    index = 0;
    if (!get_next_token(line, &index, token)) {
        return 0;/* blank line*/
    }
    if (strcmp("print_comp", token) == 0) {
        char c;
        if (!read_in_format(line, &index, token, "c", &c)) {
            return 0;
        }
        print_complex(nums[c - 'A']);
        return 0;
    }
    if (strcmp("read_comp", token) == 0) {
        char c;
        double d1, d2;
        if (!read_in_format(line, &index, token, "c,f,f", &c, &d1, &d2)) {
            return 0;
        }
        nums[c - 'A'].real_part = d1;
        nums[c - 'A'].imaginary_part = d2;
        return 0;
    }
    if (strcmp("add_comp", token) == 0) {
        char c1, c2;
        if (!read_in_format(line, &index, token, "c,c", &c1, &c2)) {
            return 0;
        }
        print_complex(add(nums[c1 - 'A'], nums[c2 - 'A']));
        return 0;
    }
    if (strcmp("sub_comp", token) == 0) {
        char c1, c2;
        if (!read_in_format(line, &index, token, "c,c", &c1, &c2)) {
            return 0;
        }
        print_complex(sub(nums[c1 - 'A'], nums[c2 - 'A']));
        return 0;
    }
    if (strcmp("mult_comp_real", token) == 0) {
        char c;
        double d;
        if (!read_in_format(line, &index, token, "c,f", &c, &d)) {
            return 0;
        }
        print_complex(multReal(nums[c - 'A'], d));
        return 0;
    }
    if (strcmp("mult_comp_img", token) == 0) {
        char c;
        double f;
        if (!read_in_format(line, &index, token, "c,f", &c, &f)) {
            return 0;
        }
        print_complex(multImg(nums[c - 'A'], f));
        return 0;
    }
    if (strcmp("mult_comp_comp", token) == 0) {
        char c1, c2;
        if (!read_in_format(line, &index, token, "c,c", &c1, &c2)) {
            return 0;
        }
        print_complex(multComp(nums[c1 - 'A'], nums[c2 - 'A']));
        return 0;
    }
    if (strcmp("abs_comp", token) == 0) {
        char c;
        if (!read_in_format(line, &index, token, "c", &c)) {
            return 0;
        }
        print_real(comp_abs(nums[c - 'A']));
        return 0;
    }
    if (strcmp("stop", token) == 0) {
        if (!read_end_of_string(line, &index, token)) {
            return 0;
        }
        return 1;
    }
    printf("Unrecognized command name: %s", token);
    return 0;
}

int main() {
    Complex nums[NUM_OF_COMPLEXEX];
    char line[MAX_LINE_LENGTH], token[MAX_LINE_LENGTH];
    init_array(nums);
    while (fgets(line, sizeof line, stdin)) {
        puts(line);
        if (execute_command(nums, line, token)) {
            return 0;
        }
        printf("\n");
    }
    printf("end of input, stop without stop command");
    return 0;
}
