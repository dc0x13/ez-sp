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

void usage_bad_usage (char **argv, const struct argxs_res *res)
{
    common_macro_init_error_msg("arguments");

    switch (res->fatal)
    {
        case argxs_fatal_non_sense :
        case argxs_fatal_undef_flag:
            fprintf(stderr, "  not being able to parse '%s' as a program's flag, make sure\n", argv[res->argc]);
            fprintf(stderr, "  of available flags by running 'ez-sp --help' :)\n\n");
            break;

        case argxs_fatal_unnecessary_arg:
            fprintf(stderr, "  giving extra argument to a flag which already has its argument.\n");
            fprintf(stderr, "  make sure of this flag usage by running 'ez-sp --help=%s'.\n\n", res->last->flag->flagname);
            break;

        case argxs_fatal_arg_expected:
            fprintf(stderr, "  expecting an argument to be given, but got nothing instead :(.\n");
            fprintf(stderr, "  make sure of this flag usage by running 'ez-sp --help=%s'.\n\n", res->last->flag->flagname);
            break;
    }

    exit(EXIT_FAILURE);
}

void usage_provide_help (const char *with)
{
    printf("todo: %s\n", with);
}
