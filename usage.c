/*                 __
 *                / _)
 *       _.----._/ /    dc0x13
 *      /         /     part of `ez-sp` project.
 *   __/ (  | (  |      Apr 07 2025
 *  /__.-'|_|--|_|
 */
#include "usage.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void programs_usage (void);

void usage_bad_usage (char **argv, const struct argxs_res *res)
{
    common_macro_init_error_msg("arguments");

    switch (res->fatal)
    {
        case argxs_fatal_non_sense :
        case argxs_fatal_undef_flag:
            fprintf(stderr, "  not being able to parse '%s' as a program's flag, \x1b[1mmake sure\x1b[0m\n", argv[res->argc]);
            fprintf(stderr, "  of available flags by running 'ez-sp --help' :)\n\n");
            break;

        case argxs_fatal_unnecessary_arg:
            fprintf(stderr, "  giving extra argument to a flag which already has its argument.\n");
            fprintf(stderr, "  \x1b[1mmake sure\x1b[0m of this flag usage by running 'ez-sp --help=%s'.\n\n", res->last->flag->flagname);
            break;

        case argxs_fatal_arg_expected:
            fprintf(stderr, "  expecting an argument to be given, but got nothing instead :(.\n");
            fprintf(stderr, "  \x1b[1mmake sure\x1b[0m of this flag usage by running 'ez-sp --help=%s'.\n\n", res->last->flag->flagname);
            break;
    }
    exit(EXIT_FAILURE);
}

void usage_provide_help (const char *with)
{
    if (with == NULL)
    {
        programs_usage();
    }

    printf("\n  haven't implemented '%s' info page, sorry :(\n\n", with);
    exit(EXIT_FAILURE);
}

static void programs_usage (void)
{
    const char *const usage =
    "\n"
    "\x1b[1mez-sp\x1b[0m: simple spread sheet usage - %s %s\n"
    "  you can make this message appear by running 'ez-sp --help'\n\n"
    "  usage: ez-sp --document mydoc.sp [flags] -- sheet2.sp sheet3.sp\n"
    "     or: ez-sp --document mydoc.sp --format json -x fxs.spx\n\n"
    "  flags:\n"
    "    \x1b[2m-\x1b[0md or \x1b[2m--\x1b[0mdocument       specifies the main document, other documents can be given as positional arguments\n"
    "    \x1b[2m-\x1b[0mx or \x1b[2m--\x1b[0mfunctions      specifies external functions defined by the user\n"
    "    \x1b[2m-\x1b[0ms or \x1b[2m--\x1b[0mstyle          specifies output style to be applied (only ansi for format)\n"
    "    \x1b[2m-\x1b[0mf or \x1b[2m--\x1b[0mformat         specifies the format to display the output\n"
    "    \x1b[2m-\x1b[0mS or \x1b[2m--\x1b[0mseparator      specifies the separator to be used in the lexer process\n"
    "    \x1b[2m-\x1b[0mh or \x1b[2m--\x1b[0mhelp           provides help with the specified argument, if no argument is given, this message is displayed\n\n"
    "  help:\n"
    "    for more information about some flag run 'ez-sp help=<flag-name>'\n"
    "\n";
    printf(usage, __DATE__, __TIME__);
    exit(EXIT_SUCCESS);
}
