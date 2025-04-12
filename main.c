#include "lexer.h"
#include "usage.h"

#include <string.h>

static void recive_execution_args (const uint32_t, char**, struct Program*);
static size_t read_file (const char*, char**);

static void set_sheet_dimensions (struct Sheet*);

int main (int argc, char **argv)
{
    struct Program program;
    memset(&program, 0, sizeof(program));
    recive_execution_args((uint32_t) argc, argv, &program);

    program.workbook = (struct Sheet*) calloc(program.nosheets, sizeof(struct Sheet));
    macro_common_check_alloc(program.workbook, macro_common_stage_sheet_craft);

    for (uint16_t i = 0; i < program.nosheets; i++)
    {
        struct Sheet *sheet = &program.workbook[i];
        sheet->name = program.xargs.sheetnames[i];
        sheet->length = read_file(sheet->name, &sheet->source);
        set_sheet_dimensions(sheet);
    }

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

    program->nosheets++;
    for (uint32_t i = 0; i < res->no_p_args; i++)
    {
        if (program->nosheets == macro_common_max_no_sheets)
        {
            macro_common_init_error(macro_common_stage_arguments);
            fprintf(stderr, "  maximum sheets reached (%d), man what are you working on?\n", macro_common_max_no_sheets);
            fprintf(stderr, "  organize your stuff better and the come back :)\n\n");
        }
        program->xargs.sheetnames[program->nosheets++] = res->p_args[i];
    }
    argxs_clean(res);
}

static size_t read_file (const char *filename, char **source)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        macro_common_init_error(macro_common_stage_doc_reading);
        fprintf(stderr, "  trying to read: %s\n", filename);
        fprintf(stderr, "  however it was not possible, make sure file exists\n");
        fprintf(stderr, "  and it has the correct pemissions\n\n");
        exit(EXIT_FAILURE);
    }
    fseek(file, 0, SEEK_END);
    const size_t bytes = ftell(file);
    if (bytes >= 1e6)
    {
        macro_common_init_error(macro_common_stage_doc_reading);
        fprintf(stderr, "  damn bro, %s is such a huge file, i think you should\n", filename);
        fprintf(stderr, "  reconcider your life choices\n\n");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_SET);
    *source = (char*) calloc(bytes + 1, sizeof(**source));
    macro_common_check_alloc(*source, macro_common_stage_doc_reading);

    const size_t read = fread(*source, 1, bytes, file);
    if (read != bytes)
    {
        macro_common_init_error(macro_common_stage_doc_reading);
        fprintf(stderr, "  cannot read '%s' file at all, only %ld out of %ld bytes were read\n", filename, read, bytes);
        fprintf(stderr, "  program is going to abort right now :(\n\n");
        exit(EXIT_FAILURE);
    }

    fclose(file);
    return bytes;
}

static void set_sheet_dimensions (struct Sheet *sheet)
{
    uint16_t nocols = 0;

    for (size_t i = 0; i < sheet->length; i++)
    {
        const char ch = sheet->source[i];
        if (ch == macro_lexer_cell_ch_sep)
        {
            nocols++;
            continue;
        }
        if (ch == '\n')
        {
            sheet->norows++;
            sheet->nocols = (nocols > sheet->nocols) ? nocols : sheet->nocols;
            nocols = 0;
        }
    }

    sheet->nocols = (nocols > sheet->nocols) ? nocols : sheet->nocols;
    if (sheet->norows == 0 && sheet->nocols == 0 || sheet->nocols == 0)
    {
        macro_common_init_error(macro_common_stage_sheet_craft);
        fprintf(stderr, "  the sheet '%s' does not contain any content on it\n", sheet->name);
        fprintf(stderr, "  make sure your sheets have some shit\n\n");
        exit(EXIT_FAILURE);
    }
}
