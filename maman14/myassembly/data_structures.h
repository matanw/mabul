#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "list.h"

typedef struct command_line {
    int bits;
    int source_line_number;
    char *label;
} CommandLine;


typedef enum SECTION_TYPE {
    Command = 1, Data = 2, None = 0
} section_type;

typedef struct label_data {
    char *label;
    int code_address;
    section_type section_type;
} LabelData;

typedef struct shared_label {
    char *label;
    int source_line_number;
} SharedLabel;
typedef struct program_information {
    List *command_lines;
    List *data_lines;
    List *label_datas;
    List *entries;
    List *external;
    int source_line_number;
    int is_in_error;
} ProgramInformation;

#endif
