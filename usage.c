#include "usage.h"
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define macro_internal_init_man_page(s)     printf("\n  \x1b[1msheet\x1b[0m - man page for \x1b[3m%s\x1b[0m - %s %s\n\n", s, __DATE__, __TIME__)

static void show_full_usage_message (void);
static void help_with_document_flag (void);

static void help_with_usage_flag (void);

void usage_fatal_at_response (char **args, const struct argxs_res *res)
{
    macro_common_init_error(macro_common_stage_arguments);
    const enum argxs_fatal f = res->fatal;

    if (f == argxs_fatal_non_sense || f == argxs_fatal_undef_flag)
    {
        const char *const msg =
        "  unrecognized flag: %s\n"
        "  make sure you are using the correct flag; if unsure, run:\n"
        "  $ sheet --help\n\n";
        fprintf(stderr, msg, args[res->argc]);
    }
    else if (f == argxs_fatal_unnecessary_arg)
    {
        if (res->last == NULL)
        {
            const char *const msg =
            "  unexpected argument: %s\n"
            "  it does not belong to any active flag. Were you trying to use it as positional argument?\n"
            "  if so, check the correct usage with:\n"
            "  $ sheet --help positional-arguments\n\n";
            fprintf(stderr, msg, args[res->argc]);
        }
        else
        {
            const char *msg =
            "  extra argument: %s.\n"
            "  the flag '%s' already has its argument: %s.\n"
            "  remove the extra input or check the flag's usage.\n\n";
            fprintf(stderr, msg, args[res->argc], res->last->flag->flagname, res->last->argument);
        }
    }
    else
    {
        const char *msg =
        "  missing argument for the '%s' flag.\n"
        "  refer to the flag's usage with:\n"
        "  $ sheet --usage %s\n\n";
        const char *fn = res->last->flag->flagname;
        fprintf(stderr, msg, fn, fn);
    }
    exit(EXIT_FAILURE);
}

void usage_usage_perse (const char *with)
{
    if (with == NULL) { show_full_usage_message(); exit(EXIT_SUCCESS); }

    const size_t a = strlen(with);
    if (strncmp(with, "document", a) == 0) { help_with_document_flag(); }
    else if (strncmp(with, "usage", a) == 0) { help_with_usage_flag(); }

    exit(EXIT_SUCCESS);
}

static void show_full_usage_message (void)
{
    const char *const usage =
    "\n  \x1b[1msheet\x1b[0m - \x1b[3mtiny spread sheet engine\x1b[0m - %s %s\n"
    "  usage: sheet [-d document] [flags] -- [positional-arguments]\n"
    "  flags:\n"
    "    \x1b[2m-\x1b[0md or \x1b[2m--\x1b[0mdocument               indicates main sheet (mandatory)\n"
    "    \x1b[2m-\x1b[0mh or \x1b[2m--\x1b[0mhelp                   show this message or provide help (\x1b[2mi.e. --help=document\x1b[0m)\n\n";
    printf(usage, __DATE__, __TIME__);
}

static void help_with_document_flag (void)
{
    macro_internal_init_man_page("document-flag");
    const char *const msg =
    "  \033[1m--document\033[0m flag specifies the \033[1mprimary sheet\033[0m the engine will operate on.\n"
    "  To include additional sheets, pass them as \033[1mpositional arguments\033[0m after the flags.\n\n"
    "  \033[90mExample:\033[0m\n"
    "    sheet -d mysheet -- sheet1 sheet2\n\n";
    printf("%s", msg);
}

static void help_with_usage_flag (void)
{
    macro_internal_init_man_page("usage-flag");
    const char *const msg =
    "  \033[1m--usage\033[0m prints a detailed usage guide for the program.\n"
    "  If used \033[1malone\033[0m (with no arguments), it will display a general overview\n"
    "  of all available flags and usage examples.\n"
    "  If you provide an \033[1margument\033[0m (like a flag name), it will show usage info\n"
    "  specifically for that flag.\n\n"
    "  \033[90mExamples:\033[0m\n"
    "    sheet --usage\n"
    "    sheet --usage document\n\n";
    printf("%s\n", msg);
}
