#include "lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define macro_get_row_number(numberline)   (numberline - 1)
#define macro_phase                        "lexer"
#define macro_valid_first_token(kind)      ((kind == token_is_string)    || (kind == token_is_number)        ||  \
                                            (kind == token_is_true_bool) || (kind == token_is_false_bool)    ||  \
                                            (kind == token_is_const_ref) || (kind == token_is_varia_ref)     ||  \
                                            (kind == token_is_expr_init) || (kind == token_is_conditional)   ||  \
                                            (kind == token_is_clone_up))

/* To parse column names the Base25 is used
 * in the following way:
 * @abc69
 * a = 26^2 * 1
 * b = 26^1 * 2
 * c = 26^0 * 3
 */
static unsigned int *Base25;

/* Reason why the lexer failed at any possible
 * stage
 */
static const char *const WhyError[] = {
    "reference is outta bounds, check table's size",
    "token is not defined.",
    "cell has reached its limit of tokens",
    "syntax error",
    "invalid first token, make sure each cell builds an expression",
    "cannot make a reference to a cell which has not been parsed"
};

static char ShowTokensFound = __macro_dont_show_debug_info;

struct lexer_info
{
    unsigned int numberline;
    unsigned int offsetline;
    unsigned int column;
};

static void get_table_size (char*, unsigned int*, unsigned*, const char);
static void gen_base_25 (const unsigned int);

static void token_found (struct cell*, struct token*);
static size_t number_literal_found (unsigned int*, struct token*);

static size_t string_literal_found (unsigned int*, struct token*);
static size_t referece_literal_found (unsigned int*, struct token*, const unsigned int, const unsigned int, const char);

static size_t extract_number_from_source (const char*, long double*);

void lexer_init (struct program *_p, const size_t bytes)
{
    ShowTokensFound = _p->args.debug_info;

    get_table_size(_p->docstr, &_p->table.rows, &_p->table.cols, _p->args.sep);
    gen_base_25(_p->table.cols);

    _p->table.grid = (struct cell*) calloc(_p->table.cols * _p->table.rows, sizeof(struct cell));
    __macro_check_ptr(_p->table.grid, macro_phase);

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

        if (chr == _p->args.sep)
        {
            info.offsetline++;
            info.column++;
            cell++;
            continue;
        }

        /* Current token definition, its definition within the table might be wrong
         * but this is the standard definition, also used for error handling
         */
        struct token token = {
            .info.definition = _p->docstr + i,
            .info.length     = 1,
            .info.numline    = info.numberline,
            .info.offset     = info.offsetline++,
            .info.column     = info.column,
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
                break;

            case token_is_sub_sign:
                if ((i + 1 < bytes) && isdigit(_p->docstr[i + 1])) i += number_literal_found(&info.offsetline, &token);
                else                                               token.kind = token_is_sub_sign;
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
                i += number_literal_found(&info.offsetline, &token);
                break;
            
            case token_is_string:
                i += string_literal_found(&info.offsetline, &token);
                break;
            
            case token_is_const_ref:
            case token_is_varia_ref:
                i += referece_literal_found(&info.offsetline, &token, _p->table.rows, _p->table.cols, chr);
                token.as.ref.ptr = &_p->table.grid[token.as.ref.row * _p->table.cols + token.as.ref.col];
                break;
            
            default:
                lexer_highlight_error_within_source(token, src_err_is_due_to_unknown);
                break;
        }
        token_found(cell, &token);
    }
    free(Base25);
}

/* This function is also used in parser; it is used to highlight the error found.
 */
void lexer_highlight_error_within_source (const struct token token, const enum source_error_kind error)
{
    fprintf(stderr, "ez-sp: fatal error while lexing\n");
    fprintf(stderr, "token is located at %dth row and %dth column\n", macro_get_row_number(token.info.numline), token.info.column);

    unsigned int context = 0;

    while (!isspace(token.info.definition[context])) context++;
    fprintf(stderr, "  %-5.d  %.*s\n         ", token.info.numline, context, token.info.definition);

    for (unsigned int i = 1; i <= token.info.length; i++) fputc('~', stderr);

    fprintf(stderr, "\nreason: %s\n", WhyError[error]);
    exit(EXIT_FAILURE);
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

static void gen_base_25 (const unsigned int columns)
{
    Base25 = (unsigned int*) calloc(columns, sizeof(*Base25));
    __macro_check_ptr(Base25, macro_phase);

    Base25[0] = 1;

    for (unsigned int i = 1; i < columns; i++)
        Base25[i] = Base25[i - 1] * 25;
}

static void token_found (struct cell *cell, struct token *token)
{
    if (cell->streamsz == __macro_tokens_per_cell)
    { lexer_highlight_error_within_source(*token, src_err_is_due_to_cell_is_fuil); }

    if (ShowTokensFound)
    {
        printf("token found: <%d> \t\t (lemgth: %d) (numline: %d) (offset: %d) (row: %d) (column: %d)\n",
        token->kind, token->info.length, token->info.numline, token->info.offset, token->info.numline - 1, token->info.column);
    }

    if (cell->streamsz == 0 && !macro_valid_first_token(token->kind))
    { lexer_highlight_error_within_source(*token, src_err_is_due_to_invalid_first_token); }

    memcpy(&cell->stream[cell->streamsz++], token, sizeof(*token));
}

static size_t number_literal_found (unsigned int *offset, struct token *token)
{
    token->info.length = extract_number_from_source(token->info.definition, &token->as.number);
    *offset += token->info.length;

    token->kind = token_is_number;
    return (size_t) token->info.length - 1;
}

static size_t string_literal_found (unsigned int *offset, struct token *token)
{
    size_t k = 1;
    for (; token->info.definition[k] && token->info.definition[k] != '"'; k++);
    
    /* This defines the token length, not the string length.
     * string_length = token_len - 2, we need to get rid of
     * the quotes
     */
    token->info.length = k + 1;
    *offset += k;

    token->kind = token_is_string;
    token->as.text = token->info.definition;
    return k;
}

static size_t referece_literal_found (unsigned int *offset, struct token *token, const unsigned int nrows, const unsigned int ncols, const char kind)
{
    enum source_error_kind paila;
    char *src = token->info.definition + 1;

    while (isalpha(src[token->info.length]))
    { token->info.length++; }

    if (token->info.length == 0)
    { paila = src_err_is_due_to_syntax_error; goto la_madre_que_lo_pario; }

    unsigned int column = 0;
    long double row__ = 0;

    for (unsigned int i = 0; i < token->info.length; i++)
    { column += (tolower(src[i]) - 'a' + 1) * Base25[token->info.length - i - 1]; }
    
    src += token->info.length;
    if (!isdigit(*src))
    { paila = src_err_is_due_to_syntax_error; goto la_madre_que_lo_pario; }

    token->info.length += (unsigned int) extract_number_from_source(src, &row__);
    unsigned int row = (unsigned int) row__;

    if (row >= nrows || --column > ncols)
    { paila = src_err_is_due_to_bounds; goto la_madre_que_lo_pario; }

    token->as.ref.row = ++row;
    token->as.ref.col = column;
    token->kind = (kind == '@') ? token_is_varia_ref : token_is_const_ref;

    *offset += token->info.length;
    return token->info.length++;

la_madre_que_lo_pario:
    /* At the beginning @ | $ character is ignored, therefore
     * we need to include it to mark the whole token as error
     */
    token->info.length++;
    lexer_highlight_error_within_source(*token, paila);

    return 0;
}

static size_t extract_number_from_source (const char *source, long double *number)
{
    char *ends;
    *number = strtold(source, &ends);
    return (size_t) (ends - source);
}
