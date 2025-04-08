/*                 __
 *                / _)
 *       _.----._/ /    dc0x13
 *      /         /     part of `ez-sp` project.
 *   __/ (  | (  |      Apr 8 2025
 *  /__.-'|_|--|_|
 */
#include "lexer.h"

#include <ctype.h>

#define own_macro_set_coords_of(c, y, x)   do { c->row = y - 1, c->col = x; } while (0)

void lexer_lex_this_shit (struct Sheet *sheet, const char sep)
{
    uint16_t numberline = 1, offsetline = 0, column = 0;
    struct Cell *cc = &sheet->grid[0];

    for (size_t i = 0; i < sheet->length; i++)
    {
        const char this = sheet->src[i];
        if (isspace(this))
        {
            if (this != '\n' || numberline >= sheet->norows)
            { offsetline++; continue; }

            offsetline = 0;
            column = 0;
            cc = &sheet->grid[sheet->nocols * numberline];

            own_macro_set_coords_of(cc, ++numberline, column);
            continue;
        }
        if (this == sep && (column + 1) < sheet->nocols)
        {
            offsetline++;
            cc++;
            own_macro_set_coords_of(cc, numberline, ++column);
            continue;
        }
        offsetline++;
    }

    printf("'%s' sheet:\n", sheet->name);
    for (uint16_t i = 0; i < sheet->norows; i++)
    {
        for (uint16_t j = 0; j < sheet->nocols; j++)
        {
            struct Cell *cc = &sheet->grid[i * sheet->nocols + j];
            printf("%d %d\n", cc->row, cc->col);
        }
        puts("~~~~~~~~~~~~~~~~~~~");
    }

    printf("\n\n");
}
