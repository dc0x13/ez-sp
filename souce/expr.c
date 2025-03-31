/*                 __
 *                / _)
 *       _.----._/ /    dc0x13
 *      /         /     part of `ez-sp` project.
 *   __/ (  | (  |      Mar 22 2025
 *  /__.-'|_|--|_|
 */

#include "expr.h"
#include "lexer.h"

#include <alloca.h>

#define macro_dont_swap 0
#define macro_do_swap   1

static void push_into_shunting (struct token**, struct token*, unsigned int*);
static void push_into_stack (struct token**, struct token**, struct token*, unsigned int*, unsigned int*);

static unsigned char should_swap (const enum token_kind, const enum token_kind);

long double expression_execute (struct cell *cell)
{
    struct token **shunt = (struct token**) alloca(__macro_tokens_per_cell * sizeof(struct token*));
    struct token **stack = (struct token**) alloca((__macro_tokens_per_cell >> 1) * sizeof(struct token*));

    unsigned int shnsz = 0, stcksz = 0;

    for (unsigned int i = 1; i < cell->streamsz; i++)
    {
        struct token *T = &cell->stream[i];
        switch (T->kind)
        {
            // TODO: missing references
            case token_is_number  : push_into_shunting(shunt, T, &shnsz); break;
            case token_is_sub_sign:
            case token_is_add_sign:
            case token_is_mul_sign:
            case token_is_div_sign:
            case token_is_lhs_par :
            case token_is_rhs_par : push_into_stack(stack, shunt, T, &stcksz, &shnsz); break;
            default: lexer_highlight_error_within_source(*T, src_err_is_due_to_invalid_token_within_formula); break;
        }
    }

    while (stcksz)
    {
        struct token *top = stack[--stcksz];
        push_into_shunting(shunt, top, &shnsz);
    }

    for (unsigned int i = 0; i < shnsz; i++)
    {
        if (shunt[i]->kind == token_is_number)
        {
            printf("%Lf ", shunt[i]->as.number);
        }
        else
        {
            printf("%c ", shunt[i]->kind);
        }
    }

    printf("\n");

    return 0.0f;
}

static void push_into_shunting (struct token **shunt, struct token *token, unsigned int *at)
{
    if (*at == __macro_tokens_per_cell)
    { lexer_highlight_error_within_source(*token, src_err_is_due_to_formula_overflow); }
    shunt[*at] = token;
    *at += 1;
}

static void push_into_stack (struct token **stack, struct token **shunt, struct token *token, unsigned int *stsz, unsigned int *shsz)
{
    static unsigned int lim = __macro_tokens_per_cell >> 1;
    if (*stsz == lim)
    { lexer_highlight_error_within_source(*token, src_err_is_due_to_formula_overflow); }

    if (*stsz == 0 || token->kind == token_is_lhs_par)
    { goto push_anyway; }

    if (token->kind == token_is_rhs_par)
    {
        return;
    }
    while (*stsz > 0)
    {
        struct token *top = stack[*stsz - 1];
        const unsigned char swap = should_swap(top->kind, token->kind);
        
        if (swap == macro_dont_swap)
        {
            stack[*stsz] = token;
            *stsz += 1;
            break;
        }
        push_into_shunting(shunt, top, shsz);
        *stsz -= 1;
    }

    if (*stsz == 0) goto push_anyway;
    return;

    push_anyway:
    {
        stack[*stsz] = token;
        *stsz += 1;
    }
}

static unsigned char should_swap (const enum token_kind top, const enum token_kind ths)
{
    static const unsigned short same[] =
    {
        token_is_sub_sign * token_is_add_sign,
        token_is_div_sign * token_is_mul_sign,
    };

    unsigned short product = top * ths;
    if ((product == same[0]) || (product == same[1]) || (top == ths)) return macro_do_swap;
    if (((top == token_is_mul_sign) || (top == token_is_div_sign)) && top != ths) return macro_do_swap;

    return macro_dont_swap;
}


int main ()
{
    // 3 * 8 - 2
    struct cell cell =
    {
        .stream = {
            { .kind = token_is_expr_init },
            { .kind = token_is_number,  .as.number = 3 },
            { .kind = token_is_mul_sign,.as.number = 0 },
            { .kind = token_is_number,  .as.number = 8 },
            { .kind = token_is_sub_sign,.as.number = 0 },
            { .kind = token_is_number,  .as.number = 2 },
        },
        .streamsz = 6
    };

    expression_execute(&cell);
    return 0;
}

