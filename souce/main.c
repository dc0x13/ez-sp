/*                 __
 *                / _)
 *       _.----._/ /    dc0x13
 *      /         /     part of `ez-sp` project.
 *   __/ (  | (  |      Mar 22 2025
 *  /__.-'|_|--|_|
 */

#include "lexer.h"
#include "argxs.h"
#include "usage.h"
#include "expr.h"

#include <stdlib.h>
#include <string.h>

static void parse_arguments (const unsigned int, char**, struct program*);
static void handle_argxs_error (char**, const struct argxs_res*);

static size_t read_file_content (const char*, char**);
static void process_table (struct program*);

static void set_literal_number_to_cell (struct cell*, const long double);
static void set_literal_string_to_cell (struct cell*, const char*, const unsigned int);

static void set_literal_boolean_to_cell (struct cell*, const char);
static void set_cell_as_another_cell (struct cell*, struct token *token);

int main (int argc, char **argv)
{
    struct program p = { .args.sep  = '|' };
    parse_arguments(argc, argv, &p);

    if (p.args.doc == NULL) usage_usage(NULL);
    const size_t bytes = read_file_content(p.args.doc, &p.docstr);

    lexer_init(&p, bytes);
    process_table(&p);
    return 0;
}

static void parse_arguments (const unsigned int argc, char **argv, struct program *doc)
{
    const struct argxs_flag flags[] = {
        {"doc",   'D', ARGXS_ARG_YES},
        {"sep",   'S', ARGXS_ARG_MAY},
        {"fmt",   'F', ARGXS_ARG_MAY},
        {"out",   'O', ARGXS_ARG_MAY},
        {"help",  'h', ARGXS_ARG_MAY},
        {"style", 's', ARGXS_ARG_MAY},
        {"debug", 'd', ARGXS_ARG_NON},
        ARGXS_FINAL_FLAG
    };

    struct argxs_res *res = argxs_parse(argc, argv, flags);
    if (res->fatal != argxs_fatal_none) handle_argxs_error(argv, res);

    for (unsigned int i = 0; i < res->no_found; i++)
    {
        const struct argxs_found *this = &res->found[i];
        switch (this->flag->id)
        {
            case 'D': doc->args.doc        = this->argument;  break;
            case 'S': doc->args.sep        = *this->argument; break;
            case 'F': doc->args.fmt        = this->argument;  break;
            case 'O': doc->args.out        = this->argument;  break;
            case 's': doc->args.style      = this->argument;  break;
            case 'd': doc->args.debug_info = __macro_show_debug_info; break;
            case 'h': usage_usage(this->argument); break;
        }
    }

    argxs_clean(res);
}

static void handle_argxs_error (char **argv, const struct argxs_res *res)
{
    fprintf(stderr, "ez-sp: Fatal error, cannot continue: %s\n", argxs_errors[res->fatal]);
    switch (res->fatal)
    {
        case argxs_fatal_non_sense:
        case argxs_fatal_undef_flag:
            fprintf(stderr, "  located at %d within argv: %s\n", res->argc, argv[res->argc]);
            break;
        case argxs_fatal_unnecessary_arg:
            fprintf(stderr, "  extra argument was given at %d within argv: %s\n", res->argc, argv[res->argc]);
            fprintf(stderr, "  flag: %s\n", res->last->flag->flagname);
            break;
        case argxs_fatal_arg_expected:
            fprintf(stderr, "  missing argument at %d within argv: %s\n", res->argc, argv[res->argc]);
            fprintf(stderr, "  flag: %s\n", res->last->flag->flagname);
            break;

    }
    exit(EXIT_FAILURE);
}

static size_t read_file_content (const char *filename, char **src)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "ez-sp: Fatal error, cannot load '%s' file; make sure it exists and it has permissions to read\n", filename);
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    const size_t bytes = ftell(file);
    fseek(file, 0, SEEK_SET);

    *src = (char*) calloc(bytes + 1, sizeof(**src));
    __macro_check_ptr(src, "pre-lexer");

    const size_t read = fread(*src, 1, bytes, file);
    if (read != bytes)
    {
        fprintf(stderr, "ez-sp: Fatal error, cannot read whole file, only %ld out of %ld bytes were read\n", read, bytes);
        exit(EXIT_FAILURE);
    }

    fclose(file);
    return bytes;
}

static void process_table (struct program *_p)
{
    const unsigned int total = _p->table.rows * _p->table.cols;
    printf("TOTAL: %d\n", total);

    for (unsigned int c = 0; c < total; c++)
    {
        struct cell *cell = &_p->table.grid[c];
        //if (cell->streamsz == 0) continue;
        struct token *T = &cell->stream[0];

        printf("% 4dth HEAD: %c %d\n", c, T->kind, T->kind);
        continue;

        /*switch (T->kind)
        {
            case token_is_string     : set_literal_number_to_cell(cell, T->as.number); break;
            case token_is_number     : set_literal_string_to_cell(cell, T->info.definition, T->info.length - 2); break;

            case token_is_true_bool  :
            case token_is_false_bool : set_literal_boolean_to_cell(cell, (char) (T->kind == token_is_true_bool)); break;

            case token_is_const_ref  :
            case token_is_varia_ref  : set_cell_as_another_cell(cell, T); break;

            case token_is_expr_init  : expression_execute(cell); break;

            case token_is_conditional: break;

            case token_is_clone_up   : break;
        }*/
    }
}

static void set_literal_number_to_cell (struct cell *cell, const long double number)
{
    puts("number literal assigned");
    cell->as.number = number;
    cell->kind = cell_is_number_const;
}

static void set_literal_string_to_cell (struct cell *cell, const char *text, const unsigned int length)
{
    puts("string literal assigned");
    cell->as.text.text = (char*) text;
    cell->as.text.length = length;
    cell->kind = cell_is_string_const;
}

static void set_literal_boolean_to_cell (struct cell *cell, const char boolean)
{
    puts("boolean literal assigned");
    cell->as.boolean = boolean;
    cell->kind = (boolean == __macro_true_value) ? cell_is_true_const : cell_is_false_const;
}

static void set_cell_as_another_cell (struct cell *cell, struct token *token)
{
    struct cell *pts = token->as.ref.ptr;

    if (cell <= pts)
    { lexer_highlight_error_within_source(*token, src_err_is_due_to_far_reference); }
    memcpy(cell, pts, sizeof(*cell));
    puts("reference literal performed");
}
