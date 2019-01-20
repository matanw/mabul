
#include <string.h>
#include "first_stage.h"
#include "second_stage.h"
#include "files_writer.h"
#include "deallocation.h"

#define  DEBUG_FLAG "-d"

int get_is_debug_mode(int argc, char *argv[]) {
    int i;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], DEBUG_FLAG) == 0) {
            return 1;
        }
    }
    return 0;
}


int main(int argc, char *argv[]) {
    int i;
    int is_debug_mode;
    if (argc == 1) {
        printf("Supply file name\n");
        return 0;
    }
    is_debug_mode = get_is_debug_mode(argc, argv);
    for (i = 1; i < argc; i++) {
        ProgramInformation *program_information;
        char *file_name = argv[i];
        if (strcmp(argv[i], DEBUG_FLAG) == 0) {
            continue;
        }
        program_information = do_first_stage_for_file(file_name, is_debug_mode);
        if (program_information == NULL) {
            printf("cannot read file");
            continue;
        }
        if (program_information->is_in_error) {
            printf("There are errors in file %s , output files didn't be produced\n", file_name);
            continue;
        }
        do_second_stage_for_file(program_information);

        if (program_information->is_in_error) {
            printf("There are errors in file %s , output files didn't be produced\n", file_name);
            continue;
        }
        write_files(program_information);

        printf("File %s compiled successfully\n", file_name);
        free_program_information(program_information);
    }
}
