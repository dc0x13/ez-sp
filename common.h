#ifndef SHEET_COMMON_H
#define SHEET_COMMON_H

#define macro_common_init_error(s)      fprintf(stderr, "\n\x1b\x1b[1msheet:\x1b[0m cannot continue with the process (\x1b[31mfailed\x1b[0m at '\x1b[1m%s\x1b[0m' stage)\n", s)
#define macro_common_check_alloc(p, s)  do { if (p) break; macro_common_init_error(s); fprintf(stderr, "  bad allocation in the heap\n\n"); exit(EXIT_FAILURE); } while (0)

/* Engine constants */
#define macro_common_max_no_sheets      16

/* Stages of the program, used for error messages */
#define macro_common_stage_arguments    "arguments"
#define macro_common_stage_doc_reading  "document-reading"
#define macro_common_stage_sheet_craft  "sheet-crafting"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

struct Sheet
{
    char    *source;
    size_t  length;
    char    *name;
};

struct Program
{
    struct
    {
        char *sheetnames[macro_common_max_no_sheets];
    } xargs;

    struct   Sheet *workbook;
    uint16_t nosheets;
};

#endif
