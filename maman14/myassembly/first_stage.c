
#include "first_stage.h"
#include "constants.h"
#include "list.h"
#include "command_parse.h"
#include "debug_utils.h"
#include "bits_operations.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

FirstStageOutput *do_first_stage_for_file(FILE *file) {
    char line[MAX_LINE_LENGTH];

    FirstStageData first_stage_data;
    first_stage_data.command_lines = init_list();
    first_stage_data.data_lines = init_list();
    first_stage_data.label_datas = init_list();
    first_stage_data.entries = init_list();
    first_stage_data.external = init_list();
    first_stage_data.original_line_number = 0;
    first_stage_data.is_in_error = 0;

    while (fgets(line, sizeof line, file)) {
        first_stage_data.original_line_number++;
        do_first_stage_for_line(line, &first_stage_data);
    }

    print_first_stage_data(&first_stage_data);

    free_list(first_stage_data.command_lines, (void (*)(void *))
            free_command_line_indirect);
    free_list(first_stage_data.label_datas, (void (*)(void *))
            free_label_data_indirect);
    free_list(first_stage_data.data_lines, (void (*)(void *))
            NULL);

    free_list(first_stage_data.entries, (void (*)(void *))
            NULL);
    free_list(first_stage_data.external, (void (*)(void *))
            NULL);

    return NULL;
}


int line_is_comment(char *line, int *index) {
    return expect_next_char(line, index, ';');
}

void do_first_stage_for_line(char *line, FirstStageData *first_stage_data) {
    int index;
    char token[MAX_LINE_LENGTH];
    index = 0;
    if (line_is_comment(line, &index)) {
        return;
    }
    if (!get_next_token(line, &index, token))/*blank line*/
    {
        return;
    }
    handle_label(token, line, &index, first_stage_data);
    if (!get_next_token(line, &index, token))/*blank line*/
    {
        return;
    }
    if (strcmp(token, NUMBERS_PREFIX) == 0) {
        handle_numbers(token, line, &index, first_stage_data);
    } else if (strcmp(token, STRING_PREFIX) == 0) {
        handle_string(token, line, &index, first_stage_data);
    } else if (strcmp(token, ENTRY_PREFIX) == 0) {
        handle_shared_label(line, token, &index, first_stage_data->entries, first_stage_data);
    } else if (strcmp(token, EXTERNAL_PREFIX) == 0) {
        handle_shared_label(line, token, &index, first_stage_data->external, first_stage_data);
    } else {
        handle_operation(token, line, &index, first_stage_data);
    }
}

void handle_shared_label(char *line, char *place_to_token, int *index, List *list, FirstStageData *first_stage_data) {
    if (!get_next_token(line, index, place_to_token)) {
        printf("expected label name, found end of string");
        first_stage_data->is_in_error = 1;
        return;
    }
    if (!is_legal_label(place_to_token)) {
        printf("'%s' is not a legal label", place_to_token);
        first_stage_data->is_in_error = 1;
        return;
    }
    add(list, get_shared_label(get_string_copy(place_to_token), first_stage_data->original_line_number));
    if (get_next_token(line, index, place_to_token)) {
        printf("expected end of string but found '%s'", place_to_token);
        first_stage_data->is_in_error = 1;
        return;
    }
}

void handle_label(char *token, char *line, int *index, FirstStageData *first_stage_data) {
    LabelData *label_data;
    if (!expect_next_char(line, index, ':')) {
        *index = 0;/*return index to start of string*/
        return;
    }
    printf("label:[%s]", token);
    if (!is_legal_label(token)) {
        printf("'%s' is not a legal label", token);
        first_stage_data->is_in_error = 1;
        return;
    }
    if (search(first_stage_data->label_datas, token, (int (*)(void *, void *)) compare_label_data_to_string) != NULL) {
        printf("'%s' already exists", token);
        first_stage_data->is_in_error = 1;
        return;
    }
    label_data = get_label_data(get_string_copy(token), first_stage_data->command_lines->count);
    add(first_stage_data->label_datas, label_data);
}

void handle_numbers(char *place_to_token, char *line, int *index, FirstStageData *first_stage_data) {
    int num;
    if (!get_next_token(line, index, place_to_token)) {
        printf("expected number, but found end of string");
        first_stage_data->is_in_error = 1;
        return;
    }
    if (!parse_number(place_to_token, &num)) {
        printf("'%s' is not a number", place_to_token);
        first_stage_data->is_in_error = 1;
        return;
    }
    add(first_stage_data->data_lines, get_copy_of_int(num));
    while (1) {
        if (!get_next_token(line, index, place_to_token)) {
            /*end to string, valid state*/
            return;
        }
        if (strcmp(place_to_token, ",") != 0) {
            printf("expected comma but '%s' was found", place_to_token);
            first_stage_data->is_in_error = 1;
            return;
        }

        if (!get_next_token(line, index, place_to_token)) {
            printf("expected number, but found end of string");
            first_stage_data->is_in_error = 1;
            return;
        }
        if (!parse_number(place_to_token, &num)) {
            printf("'%s' is not a number", place_to_token);
            first_stage_data->is_in_error = 1;
            return;
        }
        add(first_stage_data->data_lines, get_copy_of_int(num));
    }
}

void handle_string(char *place_to_token, char *line, int *index, FirstStageData *first_stage_data) {
    char *c;
    if (!get_string(line, index, place_to_token)) {
        printf("error while reading string");
        first_stage_data->is_in_error = 1;
        return;
    }
    if (get_next_token(line, index, place_to_token)) {
        printf("expected end of line after string, but found '%s'", place_to_token);
        first_stage_data->is_in_error = 1;
        return;
    }
    c = place_to_token;
    printf("\n token-[%s]\n", place_to_token);
    while (*c) {
        add(first_stage_data->data_lines, get_copy_of_int((int) *c));
        c++;
    }
}

void handle_operation(char *token, char *line, int *index, FirstStageData *first_stage_data) {
    operation op;
    int arguments_num;
    op = get_operation(token);
    if (op == Unknown) {
        printf("command not found :%s\n", token);
        first_stage_data->is_in_error = 1;
        return;
    }
    arguments_num = get_arguments_number(op);
    switch (arguments_num) {
        case 2:
            handle_operation_with_2_arguments(op, line, index, first_stage_data);
            break;
        case 1:
            handle_operation_with_1_argument(op, line, index, first_stage_data);
            break;
        default:
            handle_operation_without_arguments(op, line, index, first_stage_data);
    }
}

int get_arguments_number(operation op) {
    switch (op) {
        case Mov:
        case Cmp:
        case Add:
        case Sub:
        case Lea:
            return 2;
        case Not:
        case Clr:
        case Inc:
        case Dec:
        case Jmp:
        case Bne:
        case Red:
        case Prn:
        case Jsr:
            return 1;
        default:
            return 0;
    }
}

void handle_operation_with_2_arguments(operation op, char *line, int *index, FirstStageData *first_stage_data) {
    char source_argument[MAX_LINE_LENGTH];
    char target_argument[MAX_LINE_LENGTH];
    int command_bits;
    ArgumentDetails source_argument_details, target_argument_details;
    if (!read_two_arguments(line, index, source_argument, target_argument)) {
        printf("except to 2 arguments but there aren't");
        first_stage_data->is_in_error = 1;
        return;
    }
    if (!fill_argument_details(source_argument, &source_argument_details)) {
        printf(" '%s' is not register, number, or valid label name\n", source_argument);
        first_stage_data->is_in_error = 1;
        return;
    }
    if (!fill_argument_details(target_argument, &target_argument_details)) {
        printf(" '%s' is not register, number, or valid label name\n", target_argument);
        first_stage_data->is_in_error = 1;
        return;
    }

    if (get_next_token(line, index, source_argument)) {/*todo:another var?*/
        printf(" excepted end of line but found %s\n", source_argument);
        first_stage_data->is_in_error = 1;
        return;
    }
    command_bits = get_command_bits(op, &source_argument_details, &target_argument_details, 0);/*todo: are*/
    add(first_stage_data->command_lines, get_command_line(command_bits, first_stage_data->original_line_number, NULL));
    if (source_argument_details.ad_method == RegisterAddressing &&
        target_argument_details.ad_method == RegisterAddressing) {
        int registers_bits = get_two_registers_bits(&source_argument_details,
                                                    &target_argument_details);
        add(first_stage_data->command_lines,
            get_command_line(registers_bits, first_stage_data->original_line_number, NULL));
    } else {
        int source_bits, target_bits;
        source_bits = get_argument_bits(&source_argument_details, 1);
        add(first_stage_data->command_lines,
            get_command_line(source_bits, first_stage_data->original_line_number,
                             source_argument_details.label));
        target_bits = get_argument_bits(&target_argument_details, 0);
        add(first_stage_data->command_lines,
            get_command_line(target_bits, first_stage_data->original_line_number,
                             target_argument_details.label));
    }
}

void handle_operation_with_1_argument(operation op, char *line, int *index, FirstStageData *first_stage_data) {
    char argument[MAX_LINE_LENGTH];
    int command_bits, argument_bits;
    ArgumentDetails argument_details;
    if (!get_next_token(line, index, argument)) {
        printf("except to argument but there isn't");
        first_stage_data->is_in_error = 1;
        return;
    }
    if (!fill_argument_details(argument, &argument_details)) {
        printf(" '%s' is not register, number, or valid label name\n", argument);
        first_stage_data->is_in_error = 1;
        return;
    }
    if (get_next_token(line, index, argument)) {/*todo:another var?*/
        printf(" excepted end of line but found %s\n", argument);
        first_stage_data->is_in_error = 1;
        return;
    }
    command_bits = get_command_bits(op, NULL, &argument_details, 0);/*todo: are*/
    add(first_stage_data->command_lines, get_command_line(command_bits, first_stage_data->original_line_number, NULL));
    argument_bits = get_argument_bits(&argument_details, 0);
    add(first_stage_data->command_lines,
        get_command_line(argument_bits, first_stage_data->original_line_number,
                         argument_details.label));
}

void handle_operation_without_arguments(operation op, char *line, int *index, FirstStageData *first_stage_data) {
    char text_in_end_of_line[MAX_LINE_LENGTH];
    int command_bits;

    if (get_next_token(line, index, text_in_end_of_line)) {
        printf(" excepted end of line but found %s\n", text_in_end_of_line);
        first_stage_data->is_in_error = 1;
        return;
    }
    command_bits = get_command_bits(op, NULL, NULL, 0);/*todo: are*/
    add(first_stage_data->command_lines, get_command_line(command_bits, first_stage_data->original_line_number, NULL));
}

CommandLine *get_command_line(int bits, int original_line_number, char *label) {
    CommandLine *command_line = (CommandLine *) malloc(sizeof(CommandLine));
    command_line->bits = bits;
    command_line->original_line_number = original_line_number;
    command_line->label = label;
    return command_line;
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
    if (strcmp("clr", op) == 0) {
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
        argument_details->label = NULL;
        return 1;
    }
    if (parse_number(token, &(argument_details->num))) {
        argument_details->ad_method = ImmediateAddressing;
        argument_details->label = NULL;
        return 1;
    }
    if (is_legal_label(token)) {
        argument_details->ad_method = DirectAddressing;
        argument_details->num = 0;
        argument_details->label = get_string_copy(token);
        return 1;
    }
    return 0;
}

char *get_string_copy(char *str) {/*todo:move*/
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

SharedLabel *get_shared_label(char *label, int original_code_address) {
    SharedLabel *sharedLabel;
    sharedLabel = malloc(sizeof(sharedLabel));
    sharedLabel->label = label;
    sharedLabel->original_code_address = original_code_address;
    return sharedLabel;
}

int *get_copy_of_int(int num) {
    int *res = malloc(sizeof(int));
    *res = num;
    return res;
}

int get_command_bits(operation op, ArgumentDetails *source_argument_details,
                     ArgumentDetails *target_argument_details, int are) {
    int bits = 0;
    put_bits_int(&bits, op, OP_CODE_POSITION);
    if (source_argument_details != NULL) {
        put_bits_int(&bits, source_argument_details
                ->ad_method, SOURCE_ADDRESSING_POSITION);
        put_bits_int(&bits, op, OP_CODE_POSITION);
    }
    if (target_argument_details != NULL) {
        put_bits_int(&bits, target_argument_details
                ->ad_method, TARGET_ADDRESSING_POSITION);
        put_bits_int(&bits, are, ARE_POSITION);
    }
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
    } else if (argument_details->ad_method == RegisterAddressing) {
        put_bits_int(&bits, argument_details
                ->ad_method, (is_source ? SOURCE_REGISTER_POSITION : TARGET_REGISTER_POSITION));
    }
    put_bits_int(&bits, 0, ARE_POSITION);/*todo:*/
    return bits;
}


/*todo:move*/
void free_label_data_indirect(LabelData *label_data) {
    if (label_data->label != NULL)
        free(label_data->label);
}

void free_command_line_indirect(CommandLine *command_line) {
    if (command_line->label != NULL)
        free(command_line->label);
}
