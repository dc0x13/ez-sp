#include "lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define macro_get_row_number(numberline)   (numberline - 1)

struct lexer_info
{
    unsigned int numberline;
    unsigned int offsetline;
    unsigned int column;
};

static void get_table_size (char*, unsigned int*, unsigned*, const char);
static void token_found (struct cell*, struct token*);

void lexer_ (struct program *_p, const size_t bytes)
{
    get_table_size(_p->docstr, &_p->table.rows, &_p->table.cols, _p->args.sep);
    _p->table.grid = (struct cell*) calloc(_p->table.cols * _p->table.rows, sizeof(struct cell));
    assert(_p->table.grid && "internal error; cannot alloc");

    struct lexer_info info = {
        .numberline = 1,
        .offsetline = 0,
        .column     = 0,
    };

    /* this variable points to the current cell being lexed, any time a
     * seperator is found the pointer will increase by one cell, in other
     * words, it will point to the next cell, if a newline is found the pointer
     * will advance to the first cell of the next row
     */
    struct cell *cell = &_p->table.grid[0];

    for (size_t i = 0; i < bytes; i++)
    {
        const char chr = _p->docstr[i];

        if (isspace(chr))
        {
            if (chr == '\n' && info.numberline < _p->table.rows)
            {
                cell = &_p->table.grid[(++info.numberline) * _p->table.cols];
                info.offsetline = 0;
                info.column = 0;
            }
            else info.offsetline++;
            continue;
        }
        if (chr == _p->args.sep) { info.offsetline++; info.column++; cell++; continue; }

        /* Current token defintion, its definition within the table might be wrong
         * but this is the standard defintion, also used for error handling
         */
        struct token token = {
            .info.def_line = _p->docstr + i,
            .info.def_len  = 1,
            .info.numline  = info.numberline,
            .info.offset   = info.offsetline++,
            .info.column   = info.column
        };

        switch (chr)
        {
            case token_is_conditional:
            case token_is_false_bool:
            case token_is_true_bool:
            case token_is_semicolon:
            case token_is_expr_init:
            case token_is_clone_up:
            case token_is_mul_sign:
            case token_is_div_sign:
            case token_is_add_sign:
            case token_is_lhs_par:
            case token_is_rhs_par:
                token.kind = (enum token_kind) chr;
                token_found(cell, &token);
                break;

            case token_is_sub_sign:
                break;

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                break;
            
            case token_is_string:
                break;
            
            case token_is_const_ref:
            case token_is_varia_ref:
                break;
            
            default:
                break;
        }
    }
}

static void get_table_size (char *src, unsigned int *rows, unsigned *cols, const char sep)
{
    unsigned int c = 0;

    while (*src)
    {
        const char this = *src++;
        if (this == '\n') { *rows += 1; *cols = *cols > c ? *cols : c; c = 0; }
        else if (this == sep) { c++; }
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

static void token_found (struct cell *cell, struct token *token)
{
    if (cell->streamsz == __macro_tokens_per_cell)
    {
        abort();
    }

    printf("token found: <%d> (lemgth: %d) (numline: %d) (offset: %d) (row: %d) (column: %d)\n",
    token->kind, token->info.def_len, token->info.numline, token->info.offset, token->info.numline - 1, token->info.column);

    memcpy(&cell->stream[cell->streamsz++], token, sizeof(*token));
}