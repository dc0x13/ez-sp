/*                 __
 *                / _)
 *       _.----._/ /    dc0x13
 *      /         /     part of `ez-sp` project.
 *   __/ (  | (  |      Apr 07 2025
 *  /__.-'|_|--|_|
 */
#include "usage.h"
#include <stdio.h>

void usage_bad_usage (const struct argxs_res *res)
{
    switch (res->fatal)
    {
        case argxs_fatal_non_sense: break;
        case argxs_fatal_undef_flag: break;
        case argxs_fatal_unnecessary_arg: break;
        case argxs_fatal_arg_expected: break;
    }
}

void usage_provide_help (const char *with)
{
    printf("todo: %s\n", with);
}
