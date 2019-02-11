#include "files_writer.h"
#include "utils.h"
#include "constants.h"


char get_base_64_char_by_code(int code) {
    return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[code];
}

void write_bits_in_base64(int *bits, FILE *object_file) {
    int higher_value = (*bits >> 6) & 63;
    int lower_value = *bits & 63;/* 63=2^6-1=111111 in binary*/
    fprintf(object_file, "%c%c\n", get_base_64_char_by_code(higher_value), get_base_64_char_by_code(lower_value));
}

void write_command_line_in_base64(CommandLine *command_line, FILE *object_file) {
    write_bits_in_base64(&command_line->bits, object_file);
}

void write_object_file_header(ProgramInformation *program_information, FILE *object_file) {
    fprintf(object_file, "%d %d\n", program_information->command_lines->count, program_information->data_lines->count);
}

int write_object_file(ProgramInformation *program_information) {
    FILE *object_file;
    object_file = open_file(program_information->file_name, OBJECT_FILE_EXTENSION, "w");
    if (object_file == NULL) {
        return 0;
    }
    write_object_file_header(program_information, object_file
    );
    for_each(program_information
                     ->command_lines, (void (*)(void *, void *)) write_command_line_in_base64,
             object_file);
    for_each(program_information
                     ->data_lines, (void (*)(void *, void *)) write_bits_in_base64,
             object_file);
    fclose(object_file);
    return 1;
}


void write_entry(Entry *entry, FILE *file) {
    fprintf(file, "%s %d\n", entry->label, entry->code_address);
}

int write_entries_file(ProgramInformation *program_information) {
    FILE *entries_file;
    if (program_information->entries->count == 0) {
        return 1;
    }
    entries_file = open_file(program_information->file_name, ENTRIES_FILE_EXTENSION, "w");
    if (entries_file == NULL) {
        return 0;
    }
    for_each(program_information
                     ->entries, (void (*)(void *, void *)) write_entry,
             entries_file);
    fclose(entries_file);
    return 1;
}

void write_external_record(ExternalRecord *external_record, FILE *file) {
    fprintf(file, "%s %d\n", external_record->label, external_record->code_address);
}

int write_externals_file(ProgramInformation *program_information) {
    FILE *externals_file;
    if (program_information->entries->count == 0) {
        return 1;
    }
    externals_file = open_file(program_information->file_name, EXTERNAL_FILE_EXTENSION, "w");
    if (externals_file == NULL) {
        return 0;
    }
    for_each(program_information
                     ->external_records, (void (*)(void *, void *)) write_external_record,
             externals_file);
    fclose(externals_file);
    return 1;
}


/*gets a valid program_information and write the out files to disk*/
int write_files(ProgramInformation *program_information) {
    write_object_file(program_information);
    write_entries_file(program_information);
    write_externals_file(program_information);
    return 1;
}

