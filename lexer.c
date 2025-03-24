#include "lexer.h"

#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

static void get_table_size (char*, unsigned int*, unsigned*, const char);

void lexer_ (struct program *_p, const size_t bytes)
{
    get_table_size(_p->docstr, &_p->table.rows, &_p->table.cols, _p->args.sep);

    _p->table.grid = (struct cell*) calloc(_p->table.cols * _p->table.rows, sizeof(struct cell));
    assert(_p->table.grid && "internal error; cannot alloc");

    for (size_t i = 0; i < bytes; i++)
    {
    }
}

static void get_table_size (char *src, unsigned int *rows, unsigned *cols, const char sep)
{
    unsigned int c = 0;

    while (*src)
    {
        const char this = *src++;
        if (this == '\n')
        {
            *rows += 1;
            *cols = *cols > c ? *cols : c;
            c = 0;
        }
        else if (this == sep)
        {
            c++;
        }
    }
    *cols = *cols > c ? *cols : c;

    if (*cols == 0)
    {
        fprintf(stderr, "ez-sp: don't get it: there are 0 columns, do you really want '%c' to be the separator?\n", sep);
        exit(EXIT_FAILURE);
    }

    if (*rows == 0)
    {
        fprintf(stderr, "ez-sp: don't get it: there is zero rows, is there any content in the table?\n");
        exit(EXIT_FAILURE);
    }
}