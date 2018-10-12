
#include "constants.h"
#include "myassembly.h"
#include "list.h"
#include "bits_operations.h"
#include "command_parse.h"
#include <stdlib.h>
#include<stdio.h>
#include <stddef.h>
#include <string.h>

CommandLine *get_commandLine(int bits, int original_line_number,char* label) {
    CommandLine *command_line = (CommandLine *) malloc(sizeof(CommandLine));
    command_line->bits = bits;
    command_line->original_line_number = original_line_number;
    command_line->label = label;
    return command_line;
}

void print_command(CommandLine *commandLine) {
    printf("%d:", commandLine->original_line_number);
    print_bits_int(commandLine->bits);
    if(commandLine->label!=NULL){
        printf(" , label %s",commandLine->label);
    }
    printf("\n");
}

void print_label_data(LabelData *label_data) {
    printf("label:%s, %d:", label_data->label, label_data->code_address);
}

int compare_label_data_to_string(LabelData *label_data, char *label) {
    return strcmp(label_data->label, label);
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
    return Unknown;
}


int fill_argument_details(char *token, ArgumentDetails *argument_details) {
    if (parse_register(token, &(argument_details->num))) {
        argument_details->ad_method = RegisterAddressing;
        argument_details->label=NULL;
        return 1;
    }
    if (parse_number(token, &(argument_details->num))) {
        argument_details->ad_method = ImmediateAddressing;
        argument_details->label=NULL;
        return 1;
    }
    if (is_legal_label(token)) {
        argument_details->ad_method = DirectAddressing;
        argument_details->num=0;
        argument_details->label=get_string_copy(token);
        return 1;
    }
    return 0;
}

char *get_string_copy(char *str) {
    char *res;
    res = malloc((strlen(str) + 1) * sizeof(char));
    strcpy(res, str);
    return res;
}

LabelData *get_label_data(char *label, int code_address) {
    LabelData *label_data;
    label_data = malloc(sizeof(LabelData));
    label_data->label = label;
    label_data->code_address = code_address;
    return label_data;
}


int process_file_1(FILE *file) {
    char line[MAX_LINE_LENGTH];
    char token[MAX_LINE_LENGTH];
    List *command_lines;
    List *label_datas;
    int original_line_number;
    int code_address;
    command_lines = init_list();
    label_datas = init_list();
    original_line_number = 0;
    code_address = INITIAL_CODE_ADDRESS;
    printf("processing\n");
    while (fgets(line, sizeof line, file)) {
        int index, command_bits;
        operation op;
        ArgumentDetails source_argument_details, target_argument_details;
        index = 0;
        original_line_number++;
        printf("%d:[%s]\n", original_line_number, line);
        if (!get_next_token(line, &index, token))/*blank line*/
        {
            continue;
        }
        if (expect_next_char(line, &index, ':')) {
            LabelData *label_data;
            printf("label:[%s]", token);
            if (!is_legal_label(token)) {
                printf("'%s' is not a legal label", token);
                continue; /*todo: mark error*/
            }
            if (search(label_datas, token,(int (*)(void *,void *)) compare_label_data_to_string) != NULL) {
                printf("'%s' already exists", token);
                continue; /*todo: mark error*/
            }
            label_data = get_label_data(get_string_copy(token), code_address);
            add(label_datas, label_data);
            if (!get_next_token(line, &index, token))/*blank line*/
            {
                continue;/*todo: empty label is  lagal?*/
            }
        }
        op = get_operation(token);
        if (op == Unknown) {
            printf("command not found :%s\n", token);
            continue; /*todo: mark error*/
        }
        if (!get_next_token(line, &index, token)) {
            printf(" excepted token but found '%s'\n", token);
            continue; /*todo: mark error*/
        }
        if (!fill_argument_details(token, &source_argument_details)) {
            printf(" '%s' is not register, number, or valid label name\n", token);
            continue; /*todo: mark error*/
        }
        if (!read_comma(line, &index, token)) {
            printf(" excepted comma but found :%s\n", token);
            continue; /*todo: mark error*/
        }
        if (!get_next_token(line, &index, token)) {
            printf(" excepted token but found '%s'\n", token);
            continue; /*todo: mark error*/
        }
        if (!fill_argument_details(token, &target_argument_details)) {
            printf(" '%s' is not register, number, or valid label name\n", token);
            continue; /*todo: mark error*/
        }

        if (get_next_token(line, &index, token)) {
            printf(" excepted end of line but found %s\n", token);
            continue; /*todo: mark error*/
        }
        printf("op:%d\n", op);
        printf("0: am %d num %d\n", source_argument_details.ad_method, source_argument_details.num);
        printf("1: am %d num %d\n", target_argument_details.ad_method, target_argument_details.num);
        command_bits = get_command_bits(op, &source_argument_details, &target_argument_details, 0);/*todo: are*/
        add(command_lines, get_commandLine(command_bits, original_line_number,NULL));
        code_address++;
        if (source_argument_details.ad_method == RegisterAddressing &&
            target_argument_details.ad_method == RegisterAddressing) {
            int registers_bits = get_two_registers_bits(&source_argument_details,
                                                        &target_argument_details);
            add(command_lines, get_commandLine(registers_bits, original_line_number,NULL));
            code_address++;
        } else {
            int source_bits, target_bits;
            source_bits = get_argument_bits(&source_argument_details, 1);
            add(command_lines, get_commandLine(source_bits, original_line_number,source_argument_details.label));
            code_address++;
            target_bits = get_argument_bits(&target_argument_details, 0);
            add(command_lines, get_commandLine(target_bits, original_line_number,target_argument_details.label));
            code_address++;
        }
    }
    for_each(command_lines, (void (*)(void *)) print_command);
    for_each(label_datas, (void (*)(void *)) print_label_data);
    return 0;/*todo*/
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
    put_bits_int(&bits, 0, ARE_POSITION);/*todo:*/
    return bits;
}

int get_argument_bits(ArgumentDetails *argument_details,
                                int is_source) {
    int bits = 0;
    if (argument_details->ad_method == ImmediateAddressing) {
        put_bits_int(&bits, argument_details
                ->num, NUMBER_POSITION);
    } else if(argument_details->ad_method == RegisterAddressing) {
        put_bits_int(&bits, argument_details
                ->ad_method, (is_source ? SOURCE_REGISTER_POSITION : TARGET_REGISTER_POSITION));
    }
    put_bits_int(&bits, 0, ARE_POSITION);/*todo:*/
    return bits;
}


int main2222() {
    char bits[2];
    int a;
    printf("size of int is %lu, size of char is %lu\n",
           sizeof(int), sizeof(char));

    bits[0] = 1;
    bits[1] = 5;
    a = 0;
    print_bits_int(a);
    put_bits_int(&a, 1, 0);
    print_bits_int(a);
    put_bits_int(&a, 5, 2);
    print_bits_int(a);
    return 0;
}

int main1123(int argc, char *argv[]) {
    int i;
    if (argc == 1) {
        printf("Supply file name\n");
        return 0;
    }
    for (i = 1; i < argc; i++) {
        printf("str is [%s]\n", argv[i]);
        if (is_legal_label(argv[i])) {
            printf("label\n");
        } else {
            printf("not a l\n");
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int i;
    if (argc == 1) {
        printf("Supply file name\n");
        return 0;
    }

    for (i = 1; i < argc; i++) {
        FILE *file;
        printf("file is %s\n", argv[i]);
        file = fopen(argv[i], "r");
        if (file) {
            process_file_1(file);
            fclose(file);
        }
    }
}
