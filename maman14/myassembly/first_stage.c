
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "first_stage.h"
#include "list.h"
#include "command_parse.h"
#include "debug_utils.h"
#include "error_handling.h"
#include "utils.h"
#include "constants.h"
#include "first_stage_operation_handlers.h"
#include "first_stage_simple_handlers.h"
#include <stdio.h>


/*initialize the ProgramInformation sturct*/
ProgramInformation *init_program_information(char *file_name, int is_debug_mode) {
    ProgramInformation *program_information = malloc(sizeof(ProgramInformation));
    program_information->is_debug_mode = is_debug_mode;
    program_information->command_lines = init_list();
    program_information->data_lines = init_list();
    program_information->label_datas = init_list();
    program_information->entries = init_list();
    program_information->external = init_list();
    program_information->external_records = init_list();
    program_information->source_line_number = 0;
    program_information->is_in_error = 0;
    program_information->file_name = get_copy_of_string(file_name);
    return program_information;

}

/*return 1 if line is a comment, 0 otherwise*/
int is_line_a_comment(char *line, int *index) {
    return expect_next_char(line, index, ';');
}

/* do the first stage for a single line*/
void do_first_stage_for_line(char *line, ProgramInformation *program_information) {
    char token[MAX_LINE_LENGTH];
    int old_command_lines_count = program_information->command_lines->count;
    int old_data_lines_count = program_information->data_lines->count;
    char *label = NULL;
    section_type section_type;
    int index = 0;
    if (is_line_a_comment(line, &index)) {
        return;
    }
    if (!get_next_token(line, &index, token))/*blank line*/
    {
        return;
    }
    if (get_last_char(token) == ':') {
        label = get_copy_of_string(token);
        if (!get_next_token(line, &index, token))/*blank line*/
        {
            handle_error(program_information, program_information->source_line_number, "empty label is illegal");
            return;
        }
    }

    if (strcmp(token, NUMBERS_PREFIX) == 0) {
        handle_numbers(token, line, &index, program_information);
        section_type = Data;
    } else if (strcmp(token, STRING_PREFIX) == 0) {
        handle_string(token, line, &index, program_information);
        section_type = Data;
    } else if (strcmp(token, ENTRY_PREFIX) == 0) {
        handle_entry(line, token, &index, program_information);
        section_type = None;
    } else if (strcmp(token, EXTERNAL_PREFIX) == 0) {
        handle_extern(line, token, &index, program_information);
        section_type = None;
    } else {
        handle_operation(token, line, &index, program_information);
        section_type = Command;
    }
    handle_label(label, section_type, old_command_lines_count, old_data_lines_count, program_information);
}


/*do this first stage (מעבר ראשון) on the file.
 * first stage read the file line by line and parse every line by itself,
 * without reference to other lines*/
ProgramInformation *do_first_stage_for_file(char *file_name, int is_debug_mode) {
    char line[MAX_LINE_LENGTH];
    ProgramInformation *program_information;
    FILE *file = open_file(file_name, SOURCE_FILE_EXTENSION, "r");
    if (file == NULL) {
        return NULL;
    }
    program_information = init_program_information(file_name, is_debug_mode);

    while (fgets(line, sizeof line, file)) {
        program_information->source_line_number++;
        do_first_stage_for_line(line, program_information);
    }
    fclose(file);
    if (program_information->is_debug_mode) {
        printf("***after first stage:***\n");
        print_program_information(program_information);
    }
    return program_information;
}
