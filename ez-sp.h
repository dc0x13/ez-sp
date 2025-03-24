#ifndef EZ_SP_H
#define EZ_SP_H

#include <stdio.h>


struct ez_doc
{
    char *doc_filename;
    char *format;
    char *out_filename;
    char *stl_filename;
    char *doc_content;
    char sep;
};


#endif