
#include "first_stage.h"
#include "second_stage.h"
#include "files_writer.h"
#include "deallocation.h"


int main1123(int argc, char *argv[]) {
    int i;
    if (argc == 1) {
        printf("Supply file name\n");
        return 0;
    }
    for (i = 1; i < argc; i++) {
        printf("str is [%s]\n", argv[i]);
        if (argv[i]) {
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
        char *file_name = argv[i];
        ProgramInformation *program_information = do_first_stage_for_file(file_name);
        if (program_information == NULL) {
            printf("cannot read file");
            continue;
        }
        do_second_stage_for_file(program_information);
        write_files(program_information);
        free_program_information(program_information);
    }
}
