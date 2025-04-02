/*                 __
 *                / _)
 *       _.----._/ /    dc0x13
 *      /         /     part of `ez-sp` project.
 *   __/ (  | (  |      Apr 01 2025
 *  /__.-'|_|--|_|
 */
#include "argxs.h"
#include "lexer.h"
#include "error.h"

static void recive_execution_arguments (char**, const int32_t, struct program*);
static void handle_argxs_fatal (char**, struct argxs_res*);

int main (int argc, char **argv)
{
    struct program ezp =
    {
        .xargs.sep = __macro_lexer_default_sep,
    };
    recive_execution_arguments(argv, argc, &ezp);

    return 0;
}

 static void recive_execution_arguments (char **args, const int32_t nargs, struct program *ezp)
 {
     const struct argxs_flag flags[] =
     {
         {"doc",   'd', ARGXS_ARG_YES},
         {"sep",   's', ARGXS_ARG_YES},
         {"fmt",   'f', ARGXS_ARG_YES},
         {"style", 'y', ARGXS_ARG_YES},
         {"help",  'h', ARGXS_ARG_MAY},
         {"debug", 'D', ARGXS_ARG_NON},
     };

     const struct argxs_res *res = argxs_parse(nargs, args, flags);
     if (res->fatal ==  argxs_fatal_none)
     { handle_argxs_fatal(args, res); }

     for (uint32_t i = 0; i < res->no_found; i++)
     {
         switch (res->found[i].flag->id)
         {
         }
     }
 }
