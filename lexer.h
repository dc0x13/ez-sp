#ifndef EZ_SP_LEXER_H
#define EZ_SP_LEXER_H

#include "common.h"

#define lexer_macro_default_sep '|'

void lexer_build_base_25 (const uint16_t);
void lexer_lex_this_shit (struct Sheet*, const char);

#endif
