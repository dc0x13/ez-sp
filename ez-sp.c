#include "ez-sp.h"
#include "argxs.h"

static void parse_arguments (const unsigned int, char**, struct ez_doc*);

int main (int argc, char **argv)
{
    struct ez_doc doc = { .sep  = '|' };
    parse_arguments(argc, argv, &doc);

    return 0;
}

static void parse_arguments (const unsigned int argc, char **argv, struct ez_doc *doc)
{
    const struct argxs_flag flags[] = {
        {"doc", 'D', ARGXS_ARG_YES},
        {"sep", 'S', ARGXS_ARG_MAY},
        {"fmt", 'F', ARGXS_ARG_MAY},
        {"out", 'O', ARGXS_ARG_MAY},
        ARGXS_FINAL_FLAG
    };

    const struct argxs_res *res = argxs_parse(argc, argv, flags);
    if (res->fatal != argxs_fatal_none)
    {
    }

    for (unsigned int i = 0; i < res->no_found; i++)
    {
        const struct argxs_found *this = &res->found[i];
        switch (this->flag->id)
        {
            case 'D': doc->doc_filename = this->argument; break;
            case 'S': doc->sep          = *this->argument; break;
            case 'F': doc->fmt_filename = this->argument; break;
            case 'O': doc->out_filename = this->argument; break;
        }
    }

    printf("%s %s %s %c\n", doc->doc_filename, doc->fmt_filename, doc->out_filename, doc->sep);
}
