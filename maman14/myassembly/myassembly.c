
#include "first_stage.h"


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
        FILE *file;
        printf("file is %s\n", argv[i]);
        file = fopen(argv[i], "r");
        if (file) {
            do_first_stage_for_file(file);
            fclose(file);
        }
    }
}
