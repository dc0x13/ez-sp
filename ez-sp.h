#ifndef EZ_SP_H
#define EZ_SP_H

#include <stdio.h>

enum TokenKind
{
    token_is_string      = '"',
    token_is_varia_ref   = '@',
    token_is_const_ref   = '$',
    token_is_expr_init   = '=',
    token_is_sub_sign    = '-',
    token_is_mul_sign    = '*',
    token_is_div_sign    = '/',
    token_is_add_sign    = '+',
    token_is_lhs_par     = '(',
    token_is_rhs_par     = ')',
    token_is_conditional = '?',
    token_is_semicolon   = ';',
    token_is_clone_up    = '^',
    token_is_true_bool   = 'T',
    token_is_false_bool  = 'F',
    token_is_number      = 256,
    
};

enum CellKind
{
    sdfrgt
};

struct cell
{
    int a;
};

struct table
{
    struct cell  *grid;
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