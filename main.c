/*                 __
 *                / _)
 *       _.----._/ /    dc0x13
 *      /         /     part of `ez-sp` project.
 *   __/ (  | (  |      Apr 07 2025
 *  /__.-'|_|--|_|
 */
#include "lexer.h"
#include "usage.h"

static void recive_execution_args (char**, struct Program*, const uint32_t);

int main (int argc, char **argv)
{
    struct Program program = { .xargs.sep = lexer_macro_default_sep, };
    recive_execution_args(argv, &program, argc);

    return 0;
}

static void recive_execution_args (char **argv, struct Program *program, const uint32_t argc)
{
    static const struct argxs_flag flags[] = {
        {"document",  'd', ARGXS_ARG_YES},
        {"functions", 'x', ARGXS_ARG_YES},
        {"style",     's', ARGXS_ARG_YES},
        {"format",    'f', ARGXS_ARG_YES},
        {"separator", 'S', ARGXS_ARG_YES},
        {"help",      'h', ARGXS_ARG_MAY},
        ARGXS_FINAL_FLAG
    };

    struct argxs_res *res = argxs_parse(argc, argv, flags);

    if (res->fatal != argxs_fatal_none)
    {
        usage_bad_usage(res);
    }

    for (uint32_t i = 0; i < res->no_found; i++)
    {
        struct argxs_found *flg = &res->found[i];

        switch (flg->flag->id)
        {
            case 'd': program->xargs.sheetnames[program->nosheets] = flg->argument; break;
            case 'x': program->xargs.fxsfilename = flg->argument; break;
            case 's': program->xargs.stlfilename = flg->argument; break;
            case 'f': program->xargs.formatname  = flg->argument; break;
            case 'S': program->xargs.separator   = *flg->argument; break;
            case 'h': usage_provide_help(flg->argument); break;
        }
    }

    argxs_clean(res);
}
