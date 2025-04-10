/*                 __
 *                / _)
 *       _.----._/ /    dc0x13
 *      /         /     part of `ez-sp` project.
 *   __/ (  | (  |      Apr 8 2025
 *  /__.-'|_|--|_|
 */
#include "lexer.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define own_macro_set_coords_of(c, y, x)   do { c->row = y - 1, c->col = x; } while (0)

static uint64_t *Base25 = NULL;

static enum TokenKind determinate_compound_token_kind (const char, const char);
static size_t handle_reference_definition (uint16_t*, struct Token*, const uint16_t, const uint16_t);

static boolean_t look_up_for_colon_in_reference_definition (const char*, size_t*);

void lexer_build_base_25 (const uint16_t maxnocols)
{
    /* In order to avoid recalculations the engine will take the greatest number
     * of columns found in some sheet and will compute the n first powers of 25.
     */
    
    Base25 = (uint64_t*) calloc(maxnocols, sizeof(uint64_t));
    common_macro_check_alloc(Base25, common_macro_stage_analyzing_sheets);

    Base25[0] = 1;
    for (uint16_t i = 1; i < maxnocols; i++)
        Base25[i] = Base25[i - 1] * 25;
}

void lexer_lex_this_shit (struct Sheet *sheet, const char sep)
{
    uint16_t numberline = 1, offsetline = 0, column = 0;
    struct Cell *cc = &sheet->grid[0];

    for (size_t i = 0; i < sheet->length; i++)
    {
        const char this = sheet->src[i];
        if (isspace(this))
        {
            if (this != '\n' || numberline >= sheet->norows)
            { offsetline++; continue; }

            offsetline = 0;
            column = 0;
            cc = &sheet->grid[sheet->nocols * numberline];

            own_macro_set_coords_of(cc, ++numberline, column);
            continue;
        }
        if (this == sep && (column + 1) < sheet->nocols)
        {
            offsetline++;
            cc++;
            own_macro_set_coords_of(cc, numberline, ++column);
            continue;
        }

        struct Token token = 
        {
            .definition = sheet->src + i,
            .noline     = numberline,
            .offset     = offsetline++,
            .length     = 1,
        };

        switch (this)
        {
            case token_is_clone_up:
            case token_is_addition_sign:
            case token_is_division_sign:
            case token_is_lt_parentheses:
            case token_is_rt_parentheses:
            case token_is_subtraction_sign:
            case token_is_conditional_sign:
            case token_is_multiplication_sign:
                token.kind = (enum TokenKind) this;
                break;

            case token_is_expression_sign:
            case token_is_less_than_sign:
            case token_is_greater_than_sign:
                token.kind = ((i + 1) < sheet->length) ? determinate_compound_token_kind(this, sheet->src[i + 1]) : (enum TokenKind) this;
                break;

            case token_is_const_reference:
            case token_is_relat_reference: i += handle_reference_definition(&offsetline, &token, cc->row, cc->col); break;
            case token_is_string_literal:  break;
            case token_is_number_literal:  break;

            default:
                /* TODO */
                break;
        }

        /* TODO: push token into current cell */
    }
}

static enum TokenKind determinate_compound_token_kind (const char current, const char nextone)
{
    if (current == '<')
        return (nextone == '=') ? token_is_less_eq_than_sign : token_is_less_than_sign;
    if (current == '>')
        return (nextone == '=') ? token_is_great_eq_than_sign : token_is_greater_than_sign;

    return (nextone == '=') ? token_is_equal_to_sign : token_is_expression_sign;
}

/*
 *  _____________________________________
 * / all of this code is about lexing     \
 * \ references and all what they mean... /
 *  --------------------------------------
 *         \   ^__^
 *          \  (oo)\_______
 *             (__)\       )\/\
 *                 ||----w |
 *                 ||     ||
 */
static size_t handle_reference_definition (uint16_t *offset, struct Token *token, const uint16_t row, const uint16_t col)
{
    size_t adv = 0;
    const boolean_t is_external = look_up_for_colon_in_reference_definition(token->definition + 1, &adv);

    if (is_external == true)
    {
        if (adv == 0)
        {
            common_macro_highlight_error((*token), common_macro_stage_analyzing_sheets, row, col);
            fprintf(stderr, "  bad definition of external reference, run 'ez-sp --help external-refs'\n\n");
            exit(EXIT_FAILURE);
        }
        char *extername = strtok(token->definition + 1, ":");
    }

    printf("plus: %ld\n", adv);

    return adv;
}

static boolean_t look_up_for_colon_in_reference_definition (const char *src, size_t *adv)
{
    for (; src[*adv] != ':'; *adv += 1)
    {
        if (isspace(src[*adv]) || !isalpha(src[*adv]))
            return false;
    }
    return true;
}
