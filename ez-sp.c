#include "ez-sp.h"
#include "argxs.h"
#include "usage.h"

#include <stdlib.h>
#include <assert.h>

static void parse_arguments (const unsigned int, char**, struct program*);
static void handle_argxs_error (char**, const struct argxs_res*);

static size_t read_file_contents (const char*, char**);
static void parse_document (struct program*);

static void get_table_size (char*, unsigned int*, unsigned int*, const char);

int main (int argc, char **argv)
{
    struct program doc = { .args.sep  = '|' };
    parse_arguments(argc, argv, &doc);

    if (doc.args.doc == NULL) usage_usage(NULL);
    parse_document(&doc);

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
        ARGXS_FINAL_FLAG
    };

    struct argxs_res *res = argxs_parse(argc, argv, flags);
    if (res->fatal != argxs_fatal_none) handle_argxs_error(argv, res);

    for (unsigned int i = 0; i < res->no_found; i++)
    {
        const struct argxs_found *this = &res->found[i];
        switch (this->flag->id)
        {
            case 'D': doc->args.doc   = this->argument;  break;
            case 'S': doc->args.sep   = *this->argument; break;
            case 'F': doc->args.fmt   = this->argument;  break;
            case 'O': doc->args.out   = this->argument;  break;
            case 's': doc->args.style = this->argument;  break;
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

static size_t read_file_contents (const char *filename, char **src)
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
    assert(*src && "internal error; cannot alloc");

    const size_t read = fread(*src, 1, bytes, file);
    if (read != bytes)
    {
        fprintf(stderr, "ez-sp: Fatal error, cannot read whole file, only %ld out of %ld bytes were read\n", read, bytes);
        exit(EXIT_FAILURE);
    }

    fclose(file);
    return bytes;
}

static void parse_document (struct program *doc)
{
    const size_t bytes = read_file_contents(doc->args.doc, &doc->docstr);
    get_table_size(doc->docstr, &doc->table.rows, &doc->table.cols, doc->args.sep);


}

static void get_table_size (char *src, unsigned int *rows, unsigned *cols, const char sep)
{
    unsigned int c = 0;

    while (*src)
    {
        const char this = *src++;
        if (this == '\n')
        {
            *rows += 1;
            *cols = *cols > c ? *cols : c;
        }
        else if (this == sep)
        {
            c++;
        }
    }
    *cols = *cols > c ? *cols : c;

    if (*cols == 0)
    {
        fprintf(stderr, "ez-sp: don't get it: there is zero columns, do you really want '%c' to be the separator?\n", sep);
        exit(EXIT_FAILURE);
    }

    if (*rows == 0)
    {
        fprintf(stderr, "ez-sp: don't get it: there is zero rows, is there any contents in the table?\n");
        exit(EXIT_FAILURE);
    }
}