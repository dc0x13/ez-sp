#include "parser.h"
#include "lexer.h"

#include <stdlib.h>
#include <string.h>

static void transfer_constant_value_to_cell (struct cell*, struct token*);
static void transfer_cell_to_cell (struct cell*, struct token*);

void parser_init (struct program *_p)
{
    for (unsigned int C = 0; C < _p->table.rows * _p->table.cols; C++)
    {
        struct cell *cell = &_p->table.grid[C];
        if (cell->streamsz == 0) continue;

        switch (cell->stream[0].kind)
        {
            case token_is_string    :
            case token_is_number    :
            case token_is_true_bool :
            case token_is_false_bool:
                transfer_constant_value_to_cell(cell, &cell->stream[0]);
                break;
            /* The concept of `variable` and `constant` are only useful while
             * cloning
             */
            case token_is_const_ref:
            case token_is_varia_ref:
                transfer_cell_to_cell(cell, &cell->stream[0]);
                break;
            case token_is_conditional:
                break;
            case token_is_clone_up:
                break;
            case token_is_expr_init:
                break;
        }
    }
}

static void transfer_constant_value_to_cell (struct cell *cell, struct token *token)
{
    if (token->kind == token_is_number)
    {
        cell->as.number = token->as.number;
        cell->kind = cell_is_number_const;
    }
    else if (token->kind == token_is_string)
    {
        cell->as.text.text   = token->info.definition + 1;
        cell->as.text.length = token->info.length - 2;
        cell->kind = cell_is_string_const;
    }
    else if (token->kind == token_is_true_bool)
    {
        cell->as.boolean = __macro_true_value;
        cell->kind = cell_is_true_const;
    }
    else
    {
        cell->as.boolean = __macro_false_value;
        cell->kind = cell_is_true_const;
    }
}

static void transfer_cell_to_cell (struct cell *cell, struct token *token)
{
    struct cell *refers2 = token->as.ref.ptr;

    if (cell <= refers2)
    { lexer_highlight_error_within_source(*token, src_err_is_due_to_far_reference); }
    memcpy(cell, refers2, sizeof(*cell));
}
