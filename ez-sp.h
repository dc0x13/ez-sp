#ifndef EZ_SP_H
#define EZ_SP_H

#include <stdio.h>


struct cell
{
    int a;
};

struct table
{
    struct cell  *cells;
    unsigned int rows;
    unsigned int cols;
};

struct program
{
    struct {
        char  *doc;
        char  *fmt;
        char  *out;
        char  *style;
        char  sep;
    } args;

    struct table  table;
    char          *docstr;
    char          sep;
};


#endif