#ifndef SHEET_COMMON_H
#define SHEET_COMMON_H

#define macro_common_init_error(s)      fprintf(stderr, "\n\x1b\x1b[1msheet:\x1b[0m cannot continue with the process (\x1b[31mfailed\x1b[0m at '\x1b[1m%s\x1b[0m' stage)\n", s)

/* Engine constants */
#define macro_common_max_no_sheets      2

/* Stages of the program, used for error messages */
#define macro_common_stage_arguments    "arguments"


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

struct Program
{
    struct
    {
        char *sheetnames[macro_common_max_no_sheets];
    } xargs;
    uint16_t nosheets;
};

#endif
