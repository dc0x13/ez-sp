/*                 __
 *                / _)
 *       _.----._/ /    dc0x13
 *      /         /     part of `ez-sp` project.
 *   __/ (  | (  |      Mar 22 2025
 *  /__.-'|_|--|_|
 */

#ifndef EZ_SP_LEXER_H
#define EZ_SP_LEXER_H

#include "ez-sp.h"

enum source_error_kind
{
    src_err_is_due_to_bounds = 0,
    src_err_is_due_to_unknown,
    src_err_is_due_to_cell_is_fuil,
    src_err_is_due_to_syntax_error,
    src_err_is_due_to_invalid_first_token,
    src_err_is_due_to_far_reference,

    src_err_is_due_to_invalid_token_within_formula,
    src_err_is_due_to_formula_overflow,
    src_err_is_due_to_unbalanced_parentheses,
    src_err_is_due_to_malformed_expression,
};

void lexer_init (struct program*, const size_t);
void lexer_highlight_error_within_source (const struct token, const enum source_error_kind);

#endif
