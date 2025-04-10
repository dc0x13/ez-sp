/*                 __
 *                / _)
 *       _.----._/ /    dc0x13
 *      /         /     part of `ez-sp` project.
 *   __/ (  | (  |      Apr 07 2025
 *  /__.-'|_|--|_|
 */
#include "lexer.h"
#include "usage.h"

#include <stdlib.h>

static void recive_execution_args (char**, struct Program*, const uint32_t);
static uint16_t create_workbook (struct Program*);

static void read_sheet_content (struct Sheet*);
static void meassure_sheet_size (struct Sheet*, const char);

int main (int argc, char **argv)
{
    struct Program program = { .xargs.separator = lexer_macro_default_sep };
    recive_execution_args(argv, &program, argc);

    uint16_t maxofxols = create_workbook(&program);
    lexer_build_base_25(maxnocols);

    for (uint8_t i = 0; i < program.nosheets; i++)
    { lexer_lex_this_shit(&program.workbook[i], program.xargs.separator); }

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
        usage_bad_usage(argv, res);
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

    for (; program->nosheets <= res->no_p_args; program->nosheets++)
    {
        if (program->nosheets == common_macro_max_no_sheets)
        {
            common_macro_init_warnr_msg(common_macro_stage_xargs_processing);
            fprintf(stderr, "  sheets from '%s' and further cannot be processed, maximum number of sheets reached.\n\n", res->p_args[program->nosheets - 1]);
            break;
        }
        program->xargs.sheetnames[program->nosheets + 1] = res->p_args[program->nosheets];
    }

    if (program->xargs.sheetnames[0] == NULL)
    {
        usage_provide_help(NULL);
    }
    argxs_clean(res);
}

static uint16_t create_workbook (struct Program *program)
{
    uint16_t maxnocols = 0;

    program->workbook = (struct Sheet*) calloc(program->nosheets, sizeof(struct Sheet));
    common_macro_check_alloc(program->workbook, common_macro_stage_sheet_crafting);

    for (uint8_t i = 0; i < program->nosheets; i++)
    {
        struct Sheet *sheet = &program->workbook[i];
        sheet->name = program->xargs.sheetnames[i];
        sheet->no   = i;
        read_sheet_content(sheet);
        meassure_sheet_size(sheet, program->xargs.separator);

        sheet->grid = (struct Cell*) calloc(sheet->norows * sheet->nocols, sizeof(struct Cell));
        common_macro_check_alloc(sheet->grid, common_macro_stage_sheet_crafting);

        maxnocols = (maxnocols > sheet->nocols) ? maxnocols : sheet->nocols;
    }

    return maxnocols;
}

static void read_sheet_content (struct Sheet *sheet)
{
    FILE *file = fopen(sheet->name, "r");

    if (file == NULL)
    {
        common_macro_init_error_msg(common_macro_stage_sheet_crafting);
        fprintf(stderr, "  the '%s' sheet does not work, make sure it exists and it\n", sheet->name);
        fprintf(stderr, "  has permissions to be read.\n\n");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    sheet->length = (size_t) ftell(file);
    fseek(file, 0, SEEK_SET);

    if (sheet->length >= 1e6)
    {
        common_macro_init_error_msg(common_macro_stage_sheet_crafting);
        fprintf(stderr, "  the '%s' sheet is way to big, this is a light engine\n", sheet->name);
        fprintf(stderr, "  try Excel... or reconsider your life choices ¯\\_(ツ)_/¯\n\n");
        exit(EXIT_FAILURE);
    }

    sheet->src = (char*) calloc(sheet->length + 1, sizeof(char*));
    common_macro_check_alloc(sheet->src, common_macro_stage_sheet_crafting);

    const size_t b_read = fread(sheet->src, 1, sheet->length, file);
    if (b_read != sheet->length)
    {
        common_macro_init_error_msg(common_macro_stage_sheet_crafting);
        fprintf(stderr, "  not being able to read '%s' sheet at all :(\n", sheet->name);
        fprintf(stderr, "  expecting to read %ldB but got %ldB instead.\n\n", sheet->length, b_read);
        exit(EXIT_FAILURE);
    }

    fclose(file);
}

static void meassure_sheet_size (struct Sheet *sheet, const char sep)
{
    uint16_t cols = 0;

    for (size_t i = 0; i < sheet->length; i++)
    {
        const char this = sheet->src[i];
        if (this == sep)
        {
            cols++;
        } else if (this == '\n')
        {
            sheet->nocols = (cols > sheet->nocols) ? cols : sheet->nocols;
            cols = 0;
            sheet->norows++;
        }
    }

    sheet->nocols = (cols > sheet->nocols) ? cols : sheet->nocols;
    if (sheet->nocols == 0 || sheet->norows == 0)
    {
        common_macro_init_error_msg(common_macro_stage_sheet_crafting);
        fprintf(stderr, "  the '%s' sheet does not have anything defined\n", sheet->name);
        fprintf(stderr, "  make sure the table is well structured.\n\n");
        exit(EXIT_FAILURE);
    }
}
