#include "files_writer.h"
#include "io_utils.h"
#include "constants.h"


char get_base_64_char_by_code(int code) {
    return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz+"[code];
}

void write_bits_in_base64(int *bits, FILE *object_file) {
    int higher_value = *bits >> 6;
    int lower_value = *bits & 63;/* 63=2^6-1=111111 in binary*/
    fprintf(object_file, "%c%c\n", get_base_64_char_by_code(higher_value), get_base_64_char_by_code(lower_value));
}

void write_command_line_in_base64(CommandLine *command_line, FILE *object_file) {
    write_bits_in_base64(&command_line->bits, object_file);
}

void write_object_file_header(ProgramInformation *program_information, FILE *object_file) {
    fprintf(object_file, "%d %d\n", program_information->command_lines->count, program_information->data_lines->count);
}

int write_files(ProgramInformation *program_information, char *file_name) {
    FILE *object_file;
    object_file = fopen_with_extension(file_name, OBJECT_FILE_EXTENSION, "w");
    if (object_file == NULL) {
        return 0;
    }
    write_object_file_header(program_information, object_file);
    for_each_with_aside_var(program_information->command_lines, (void (*)(void *, void *)) write_command_line_in_base64,
                            object_file);
    for_each_with_aside_var(program_information->data_lines, (void (*)(void *, void *)) write_bits_in_base64,
                            object_file);
    fclose(object_file);
    return 1;
}

