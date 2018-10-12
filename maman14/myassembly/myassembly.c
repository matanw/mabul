
#include "constants.h"
#include "myassembly.h"
#include "list.h"
#include<stdio.h>

#include <string.h>

CommandLine *get_commandLine(int bits, int original_line_number) {
    CommandLine *command_line = (CommandLine *) malloc(sizeof(CommandLine));
    command_line->bits = bits;
    command_line->original_line_number = original_line_number;\
    command_line->label = NULL;
    return command_line;
}

void print_command(CommandLine *commandLine) {
    printf("%d:",commandLine->original_line_number);
    print_bits_int(commandLine->bits);
}

operation get_operation(char *op) {
    if (strcmp("mov", op) == 0) {
        return Mov;
    }
    if (strcmp("cmp", op) == 0) {
        return Cmp;
    }
    if (strcmp("add", op) == 0) {
        return Add;
    }
    if (strcmp("sub", op) == 0) {
        return Sub;
    }
    if (strcmp("not", op) == 0) {
        return Not;
    }
    if (strcmp("Clr", op) == 0) {
        return Clr;
    }
    if (strcmp("lea", op) == 0) {
        return Lea;
    }
    if (strcmp("inc", op) == 0) {
        return Inc;
    }
    if (strcmp("dec", op) == 0) {
        return Dec;
    }
    if (strcmp("jmp", op) == 0) {
        return Jmp;
    }
    if (strcmp("bne", op) == 0) {
        return Bne;
    }
    if (strcmp("red", op) == 0) {
        return Red;
    }
    if (strcmp("prn", op) == 0) {
        return Prn;
    }
    if (strcmp("jsr", op) == 0) {
        return Jsr;
    }
    if (strcmp("rst", op) == 0) {
        return Rst;
    }
    if (strcmp("stop", op) == 0) {
        return Stop;
    }
    return -1;
}


int fill_argument_details(char *token, ArgumentDetails *argument_details) {
    if (parse_register(token, &(argument_details->num))) {
        argument_details->ad_method = RegisterAddressing;
        return 1;
    }
    if (parse_number(token, &(argument_details->num))) {
        argument_details->ad_method = ImmediateAddressing;
        return 1;
    }
    return 0;
}

int process_file_1(FILE *file) {
    char line[MAX_LINE_LENGTH];
    char token[MAX_LINE_LENGTH];
    printf("processing\n");
    List *command_lines = init_list();
    int original_line_number = 0;
    while (fgets(line, sizeof line, file)) {
        original_line_number++;
        printf("%d:[%s]\n", original_line_number, line);
        int code;
        code = 0;
        int index = 0;
        if (!get_next_token(line, &index, token))//blank line
        {
            continue;
        }
        if(expect_next_char(line,&index,':')){
            printf("label:[%s]",token);
            if (!get_next_token(line, &index, token))//blank line
            {
                continue;//todo: empty label is legal?
            }
        }
        operation op = get_operation(token);
        if (op == -1) {
            printf("command not found :%s\n", token);
            continue; //todo: mark error
        }
        ArgumentDetails source_argument_details, target_argument_details;
        if (!get_next_token(line, &index, token)) {
            printf(" excepted token but found '%s'\n", token);
            continue; //todo: mark error
        }
        if (!fill_argument_details(token, &source_argument_details)) {
            printf(" '%s' is not register, number, or valid label name\n", token);
            continue; //todo: mark error
        }
        if (!read_comma(line, &index, token)) {
            printf(" excepted comma but found :%s\n", token);
            continue; //todo: mark error
        }
        if (!get_next_token(line, &index, token)) {
            printf(" excepted token but found '%s'\n", token);
            continue; //todo: mark error
        }
        if (!fill_argument_details(token, &target_argument_details)) {
            printf(" '%s' is not register, number, or valid label name\n", token);
            continue; //todo: mark error
        }

        if (get_next_token(line, &index, token)) {
            printf(" excepted end of line but found %s\n", token);
            continue; //todo: mark error
        }
        printf("op:%d\n", op);
        printf("0: am %d num %d\n", source_argument_details.ad_method, source_argument_details.num);
        printf("1: am %d num %d\n", target_argument_details.ad_method, target_argument_details.num);
        int command_bits = get_command_bits(op, &source_argument_details, &target_argument_details, 0);//todo: are
        add(command_lines, get_commandLine(command_bits, original_line_number));
        if (source_argument_details.ad_method == RegisterAddressing &&
            target_argument_details.ad_method == RegisterAddressing) {
            int registers_bits = get_two_registers_bits(&source_argument_details,
                                                        &target_argument_details);
            add(command_lines, get_commandLine(registers_bits, original_line_number));
        } else{
            int source_bits=get_argument_registers_bits(&source_argument_details,1);
            add(command_lines, get_commandLine(source_bits, original_line_number));
            int target_bits=get_argument_registers_bits(&target_argument_details,0);
            add(command_lines, get_commandLine(target_bits, original_line_number));
        }
    }
    for_each(command_lines,print_command);
}

int get_command_bits(operation op, ArgumentDetails *source_argument_details,
                     ArgumentDetails *target_argument_details, int are) {
    int bits = 0;
    put_bits_int(&bits, source_argument_details
            ->ad_method, SOURCE_ADDRESSING_POSITION);
    put_bits_int(&bits, op, OP_CODE_POSITION);
    put_bits_int(&bits, target_argument_details
            ->ad_method, TARGET_ADDRESSING_POSITION);
    put_bits_int(&bits, are, ARE_POSITION);
    return bits;
}

int get_two_registers_bits(ArgumentDetails *source_argument_details,
                           ArgumentDetails *target_argument_details) {
    int bits = 0;
    put_bits_int(&bits, source_argument_details
            ->num, SOURCE_REGISTER_POSITION);
    put_bits_int(&bits, target_argument_details
            ->num, TARGET_REGISTER_POSITION);
    put_bits_int(&bits, 0, ARE_POSITION);//todo:
    return bits;
}

int get_argument_registers_bits(ArgumentDetails *argument_details,
                                int is_source) {
    int bits = 0;
    if (argument_details->ad_method == ImmediateAddressing) {
        put_bits_int(&bits, argument_details
                ->num, NUMBER_POSITION);
    } else {/*register*/
        put_bits_int(&bits, argument_details
                ->ad_method, (is_source ? SOURCE_REGISTER_POSITION : TARGET_REGISTER_POSITION));
    }
    put_bits_int(&bits, 0, ARE_POSITION);//todo:
    return bits;
}


int main2222() {
    printf("size of int is %lu, size of char is %lu\n",
           sizeof(int), sizeof(char));
    char bits[2];
    bits[0] = 1;
    bits[1] = 5;
    //print_bits(bits);
    int a;
    a = 0;
    print_bits_int(a);
    put_bits_int(&a, 1, 0);
    print_bits_int(a);
    put_bits_int(&a, 5, 2);
    print_bits_int(a);

}

int main345(int argc, char *argv[]) {
    int i;
    if (argc == 1) {
        printf("Supply file name\n");
        return 0;
    }
    for (i = 1; i < argc; i++) {
        printf("str is [%s]\n", argv[i]);
        int v;
        if (parse_number(argv[i], &v)) {
            printf("num:%d\n", v);
        } else {
            printf("not a num\n");
        }
    }
}

int main222222() {
    ArgumentDetails ad;
    ArgumentDetails *pa = &ad;
    int *pi;
    pi = &(pa->num);
    *pi = 3;
    printf("num is %d\n", ad.num);
}

int main(int argc, char *argv[]) {
    int i;
    if (argc == 1) {
        printf("Supply file name\n");
        return 0;
    }

    for (i = 1; i < argc; i++) {
        printf("file is %s\n", argv[i]);
        char str[999];
        FILE *file;
        file = fopen(argv[i], "r");
        if (file) {
            process_file_1(file);
            fclose(file);
        }
    }
}