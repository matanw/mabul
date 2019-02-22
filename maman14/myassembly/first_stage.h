#ifndef INCLUDE_FIRST_STAGE_H
#define INCLUDE_FIRST_STAGE_H

#include "data_structures.h"


/*do this first stage (מעבר ראשון) on the file.
 * first stage read the file line by line and parse every line by itself,
 * without reference to other lines*/
ProgramInformation *do_first_stage_for_file(char *file_name, int is_debug_mode);

#endif
