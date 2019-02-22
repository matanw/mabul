
#include <stdlib.h>
#include <string.h>

#include "first_stage_simple_handlers.h"
#include "error_handling.h"
#include "command_parse.h"
#include "utils.h"


/*crate a LabelData structure*/
LabelData *get_label_data(char *label, int code_address, section_type section_type,
                          int source_line_number) {
    LabelData *label_data;
    label_data = malloc(sizeof(LabelData));
    label_data->label = label;
    label_data->code_address = code_address;
    label_data->section_type = section_type;
    label_data->source_line_number = source_line_number;
    return label_data;
}


/*crate a Entry structure*/
Entry *get_entry(char *label, int source_line_number) {
    Entry *entry;
    entry = malloc(sizeof(entry));
    entry->label = label;
    entry->source_line_number = source_line_number;
    return entry;
}

/* update the ProgramInformation structure from line of  type ".entry"*/
void handle_entry(char *line, char *place_to_token, int *index, ProgramInformation *program_information) {
    if (!get_next_token(line, index, place_to_token)) {
        handle_error(program_information, program_information->source_line_number,
                     "expected label name, found end of string");
        return;
    }
    if (!is_legal_label(place_to_token)) {
        handle_error(program_information, program_information->source_line_number, "'%s' is not a legal label",
                     place_to_token);
        return;
    }
    add(program_information->entries,
        get_entry(get_copy_of_string(place_to_token), program_information->source_line_number));
    if (get_next_token(line, index, place_to_token)) {
        handle_error(program_information, program_information->source_line_number,
                     "expected end of string but found '%s'", place_to_token);
        return;
    }
}


/* update the ProgramInformation structure from line of  type ".extern"*/
void handle_extern(char *line, char *place_to_token, int *index, ProgramInformation *program_information) {
    if (!get_next_token(line, index, place_to_token)) {
        handle_error(program_information, program_information->source_line_number,
                     "expected label name, found end of string");
        return;
    }
    if (!is_legal_label(place_to_token)) {
        handle_error(program_information, program_information->source_line_number, "'%s' is not a legal label",
                     place_to_token);
        return;
    }
    add(program_information->external, get_copy_of_string(place_to_token));
    if (get_next_token(line, index, place_to_token)) {
        handle_error(program_information, program_information->source_line_number,
                     "expected end of string but found '%s'", place_to_token);
        program_information->is_in_error = 1;
        return;
    }
}


/* update the ProgramInformation structure from line with label (in addition to handle the logic of line)*/
void handle_label(char *label, section_type section_type, int old_command_lines_count, int old_data_lines_count,
                  ProgramInformation *program_information) {
    LabelData *label_data;
    int count_to_insert;
    if (label == NULL) {
        return;
    }
    delete_last_char(label);
    if (!is_legal_label(label)) {
        handle_error(program_information, program_information->source_line_number, "'%s' is not a legal label", label);
        free(label);
        return;
    }
    if (section_type == None) {
        free(label);
        return;
    }
    count_to_insert = (section_type == Command ? old_command_lines_count : old_data_lines_count);
    label_data = get_label_data(label, count_to_insert, section_type, program_information->source_line_number);
    add(program_information->label_datas, label_data);
}


/* update the ProgramInformation structure from line of  type ".data"*/
void handle_numbers(char *place_to_token, char *line, int *index, ProgramInformation *program_information) {
    int num;
    if (!get_next_token(line, index, place_to_token)) {
        handle_error(program_information, program_information->source_line_number,
                     "expected number, but found end of string");
        return;
    }
    if (!parse_number(place_to_token, &num)) {
        handle_error(program_information, program_information->source_line_number, "'%s' is not a number",
                     place_to_token);
        return;
    }
    add(program_information->data_lines, get_copy_of_int(num));
    while (1) {
        if (!get_next_token(line, index, place_to_token)) {
            /*end to string, valid state*/
            return;
        }
        if (strcmp(place_to_token, ",") != 0) {
            handle_error(program_information, program_information->source_line_number,
                         "expected comma but '%s' was found", place_to_token);
            program_information->is_in_error = 1;
            return;
        }

        if (!get_next_token(line, index, place_to_token)) {
            handle_error(program_information, program_information->source_line_number,
                         "expected number, but found end of string");
            program_information->is_in_error = 1;
            return;
        }
        if (!parse_number(place_to_token, &num)) {
            handle_error(program_information, program_information->source_line_number, "'%s' is not a number",
                         place_to_token);
            return;
        }
        add(program_information->data_lines, get_copy_of_int(num));
    }
}


/* update the ProgramInformation structure from line of  type ".string"*/
void handle_string(char *place_to_token, char *line, int *index, ProgramInformation *program_information) {
    char *c;
    if (!get_string(line, index, place_to_token)) {
        handle_error(program_information, program_information->source_line_number, "error while reading string");
        return;
    }
    if (get_next_token(line, index, place_to_token)) {
        handle_error(program_information, program_information->source_line_number,
                     "expected end of line after string, but found '%s'", place_to_token);
        return;
    }
    c = place_to_token;
    while (*c) {
        add(program_information->data_lines, get_copy_of_int((int) *c));
        c++;
    }
    add(program_information->data_lines, get_copy_of_int(0));
}

