#include "common.h"
#include "usage.h"

#include <string.h>

static void recive_execution_args (const uint32_t, char**, struct Program*);

int main (int argc, char **argv)
{
    struct Program program;
    memset(&program, 0, sizeof(program));
    recive_execution_args((uint32_t) argc, argv, &program);

    return 0;
}

static void recive_execution_args (const uint32_t argc, char **argv, struct Program *program)
{
    const struct argxs_flag flags[] =
    {
        {"document", 'd', ARGXS_ARG_YES},
        {"help",     'h', ARGXS_ARG_MAY},
        ARGXS_FINAL_FLAG,
    };

    struct argxs_res *res = argxs_parse(argc, argv, flags);

    if (res->fatal != argxs_fatal_none)
    {
        usage_fatal_at_response(argv, res);
    }
    for (uint32_t i = 0; i < res->no_found; i++)
    {
        struct argxs_found *found = &res->found[i];
        switch (found->flag->id)
        {
            case 'd': program->xargs.sheetnames[0] = found->argument; break;
            case 'h': usage_usage_perse(found->argument); break;
        }
    }
    if (program->xargs.sheetnames[0] == NULL)
    {
        usage_usage_perse("document");
    }
    for (uint32_t i = 0; i < res->no_p_args; i++)
    {
        if (i == macro_common_max_no_sheets - 1)
        {
            macro_common_init_error(macro_common_stage_arguments);
            fprintf(stderr, "  maximum sheets reached (%d), man what are you working on?\n", macro_common_max_no_sheets);
            fprintf(stderr, "  organize your stuff better and the come back :)\n\n");
        }
        program->xargs.sheetnames[i + 1] = res->p_args[i];
    }
    argxs_clean(res);
}
