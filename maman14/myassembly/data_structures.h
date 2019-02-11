#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "list.h"

/*todo: documentation*/
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
    int source_line_number;
    section_type section_type;
} LabelData;

typedef struct entry {
    char *label;
    int source_line_number;
    int code_address;
} Entry;
typedef struct external_record {
    char *label;
    int code_address;
} ExternalRecord;
typedef struct program_information {
    int is_debug_mode; /*is need torint debug prints*/
    List *command_lines; /*list of */
    List *data_lines;
    List *label_datas;
    List *entries;
    List *external;
    List *external_records;
    char *file_name;
    int source_line_number;
    int is_in_error;/* 1 if we found in error in source file, 0 otherwise*/
} ProgramInformation;

#endif
