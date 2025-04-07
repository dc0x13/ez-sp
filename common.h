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

#define common_macro_max_no_sheets  10

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

struct Sheet {
    char    *name;
    uint8_t no;
};

struct Program {
    struct {
        char    sep;
        char    *sheetnames[common_macro_max_no_sheets];
        char    *fxsfilename;
        char    *stlfilename;
        char    *formatname;
        char    separator;
    } xargs;

    uint8_t nosheets;
};

#endif
