#include "ez-sp.h"
#include "argxs.h"

#include <stdlib.h>

static void parse_arguments (const unsigned int, char**, struct ez_doc*);
static void handle_argxs_error (char**, const struct argxs_res*);

int main (int argc, char **argv)
{
    struct ez_doc doc = { .sep  = '|' };
    parse_arguments(argc, argv, &doc);

    return 0;
}

static void parse_arguments (const unsigned int argc, char **argv, struct ez_doc *doc)
{
    const struct argxs_flag flags[] = {
        {"doc", 'D', ARGXS_ARG_YES},
        {"sep", 'S', ARGXS_ARG_MAY},
        {"fmt", 'F', ARGXS_ARG_MAY},
        {"out", 'O', ARGXS_ARG_MAY},
        ARGXS_FINAL_FLAG
    };

    const struct argxs_res *res = argxs_parse(argc, argv, flags);
    if (res->fatal != argxs_fatal_none) handle_argxs_error(argv, res);

    for (unsigned int i = 0; i < res->no_found; i++)
    {
        const struct argxs_found *this = &res->found[i];
        switch (this->flag->id)
        {
            case 'D': doc->doc_filename = this->argument; break;
            case 'S': doc->sep          = *this->argument; break;
            case 'F': doc->fmt_filename = this->argument; break;
            case 'O': doc->out_filename = this->argument; break;
        }
    }
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