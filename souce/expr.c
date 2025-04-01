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

static unsigned int NoParentheses;

static void push_into_shunting (struct token**, struct token*, unsigned int*);
static void push_into_oprts (struct token**, struct token**, struct token*, unsigned int*, unsigned int*);

static unsigned char should_swap (const enum token_kind, const enum token_kind);

long double expression_execute (struct cell *cell)
{
    struct token **shunt = (struct token**) alloca(__macro_tokens_per_cell * sizeof(struct token*));
    struct token **oprts = (struct token**) alloca((__macro_tokens_per_cell >> 1) * sizeof(struct token*));

    unsigned int shnsz = 0, optsz = 0, no_operands = 0;
    NoParentheses = 0;

    for (unsigned int i = 1; i < cell->streamsz; i++)
    {
        struct token *T = &cell->stream[i];
        switch (T->kind)
        {
            // TODO: missing references
            case token_is_number  : push_into_shunting(shunt, T, &shnsz); no_operands++; break;
            case token_is_sub_sign:
            case token_is_add_sign:
            case token_is_mul_sign:
            case token_is_div_sign:
            case token_is_lhs_par :
            case token_is_rhs_par : push_into_oprts(oprts, shunt, T, &optsz, &shnsz); break;
            default: lexer_highlight_error_within_source(*T, src_err_is_due_to_invalid_token_within_formula); break;
        }
    }

    if (NoParentheses != 0)
    { lexer_highlight_error_within_source(cell->stream[0], src_err_is_due_to_unbalanced_parentheses); }

    while (optsz)
    {
        struct token *top = oprts[--optsz];
        push_into_shunting(shunt, top, &shnsz);
    }

    long double *numstack = (long double*) alloca(sizeof(long double) * no_operands);
    unsigned int length = 0;

    for (unsigned int i = 0; i < shnsz; i++)
    {
        if (shunt[i]->kind == token_is_number)
        { printf("%Lf ", shunt[i]->as.number); numstack[length++] = shunt[i]->as.number; continue; }

        if (length <= 1)
        { lexer_highlight_error_within_source(cell->stream[0], src_err_is_due_to_malformed_expression); }

        long double lhs = numstack[length - 2], rhs = numstack[length - 1], new = 0;
        length -= 2;

        printf("%c ", shunt[i]->kind);

        switch (shunt[i]->kind)
        {
            case token_is_sub_sign: new = lhs - rhs; break;
            case token_is_add_sign: new = lhs + rhs; break;
            case token_is_mul_sign: new = lhs * rhs; break;
            case token_is_div_sign: new = lhs / rhs; break;
        }

        numstack[length++] = new;
    }

    printf("\nfrom expr, ans is: %Lf\n~~~~~~~~~~~~~~~~~~~\n", numstack[0]);
    return numstack[0];
}

static void push_into_shunting (struct token **shunt, struct token *token, unsigned int *at)
{
    if (*at == __macro_tokens_per_cell)
    { lexer_highlight_error_within_source(*token, src_err_is_due_to_formula_overflow); }
    shunt[*at] = token;
    *at += 1;
}

static void push_into_oprts (struct token **oprts, struct token **shunt, struct token *token, unsigned int *optsz, unsigned int *shsz)
{
    static unsigned int lim = __macro_tokens_per_cell >> 1;
    if (*optsz == lim)
    { lexer_highlight_error_within_source(*token, src_err_is_due_to_formula_overflow); }

    if (*optsz == 0 || token->kind == token_is_lhs_par)
    { goto push_anyway; }

    if (token->kind == token_is_rhs_par)
    {
        if (NoParentheses == 0)
        { lexer_highlight_error_within_source(*token, src_err_is_due_to_unbalanced_parentheses); }

        while (1)
        {
            *optsz -= 1;
            struct token *top = oprts[*optsz];
            if (top->kind == token_is_lhs_par) break;
            push_into_shunting(shunt, top, shsz);
        }

        NoParentheses--;
        return;
    }

    while (*optsz > 0)
    {
        struct token *top = oprts[*optsz - 1];
        const unsigned char swap = should_swap(top->kind, token->kind);
        
        if (swap == macro_dont_swap)
        {
            oprts[*optsz] = token;
            *optsz += 1;
            break;
        }
        push_into_shunting(shunt, top, shsz);
        *optsz -= 1;
    }

    if (*optsz == 0) goto push_anyway;
    return;

    push_anyway:
    {
        if (token->kind == token_is_lhs_par) NoParentheses++;
        oprts[*optsz] = token;
        *optsz += 1;
    }
}

static unsigned char should_swap (const enum token_kind top, const enum token_kind ths)
{
    if (top == token_is_lhs_par) return macro_dont_swap;

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
