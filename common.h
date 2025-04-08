/*                 __
 *                / _)
 *       _.----._/ /    dc0x13
 *      /         /     part of `ez-sp` project.
 *   __/ (  | (  |      Apr 07 2025
 *  /__.-'|_|--|_|
 */
#ifndef EZ_SP_COMMON_H
#define EZ_SP_COMMON_H

#include <stdio.h>

#define common_macro_max_no_sheets       10

#define common_macro_init_error_msg(s)   fprintf(stderr, "\n\x1b[1mez-sp\x1b[0m: simple spread sheet presents an \x1b[1;31merror\x1b[0m at '%s' stage:\n\n", s)
#define common_macro_init_warnr_msg(s)   fprintf(stderr, "\n\x1b[1mez-sp\x1b[0m: simple spread sheet presents an \x1b[1;33mwarning\x1b[0m at '%s' stage:\n", s)

#define common_macro_check_alloc(ptr, s) \
  do { \
    if (ptr) break; \
    common_macro_init_error_msg(s); \
    fprintf(stderr, "  having problem to allocate memory into the heap :(\n"); \
    fprintf(stderr, "  this in an internal error\n\n"); \
  } while (0)

#define common_macro_stage_xargs_processing "execution-arguments-processing"
#define common_macro_stage_sheet_crafting   "sheet-crafting"

#define uint8_t     unsigned char
#define uint16_t    unsigned short
#define uint32_t    unsigned int
#define uint64_t    unsigned long

#define int8_t      signed char
#define int16_t     signed short
#define int32_t     signed int
#define int64_t     signed long

#define boolean_t   signed char
#define true        1
#define false       -1

struct Token {
    char      *definition;
    uint16_t  noline;
    uint16_t  offset;
};

struct Cell {
    uint16_t    col;
    uint16_t    row;
};

struct Sheet {
    struct   Cell *grid;
    char     *src;
    char     *name;
    size_t   length;
    uint16_t norows, nocols;
    uint8_t  no;
};

struct Program {
    struct {
        char    *sheetnames[common_macro_max_no_sheets];
        char    *fxsfilename;
        char    *stlfilename;
        char    *formatname;
        char    separator;
    } xargs;
    struct  Sheet *workbook;
    uint8_t nosheets;
};

#endif
