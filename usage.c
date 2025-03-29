#include "usage.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void know_more_formats (void);
static void know_more_styles (void);

static void know_more_define_style (void);

void usage_usage (const char *with)
{
    if (with == NULL) {
        printf("ez-sp: Easy Spreadsheet Engine (mostly for demonstration purposes) %s %s\n", __DATE__, __TIME__);
        printf("  Usage: ez-sp [--doc document] [flags]\n");
        printf("  Flags:\n");
        printf("    -O or --out      Specify the output destination (stdout by default).\n");
        printf("    -D or --doc      Specify the document to be parsed and analyzed.\n");
        printf("    -F or --fmt      Define the format of the output.\n");
        printf("    -S or --sep      Set the separator to be used ('|' by default).\n");
        printf("    -s or --style    Set the style to be used (No style by default).\n");
        printf("    -H or --help     Print this message or provide help for a specific flag.\n");
    }
    else if (!strncmp(with, "out", 3)) {
        printf("Usage of -O or --out:\n");
        printf("  Specifies the destination where the output should be written.\n");
        printf("  By default, the output is directed to stdout.\n");
        printf("  Example: ez-sp --out output.txt (writes output to output.txt).\n");
    }
    else if (!strncmp(with, "doc", 3)) {
        printf("Usage of -D or --doc:\n");
        printf("  Specifies the document to be parsed and analyzed.\n");
        printf("  This document should be a valid input file for ez-sp.\n");
        printf("  Example: ez-sp --doc data.csv (processes data.csv).\n");
    }
    else if (!strncmp(with, "fmt", 3)) {
        printf("Usage of -F or --fmt:\n");
        printf("  Defines the format of the output.\n");
        printf("  Example: ez-sp --fmt json (outputs results in JSON format).\n");
        printf("  Run ez-sp --help=formats to know more.\n");
    }
    else if (!strncmp(with, "sep", 3)) {
        printf("Usage of -S or --sep:\n");
        printf("  Allows specifying a custom separator for the output.\n");
        printf("  By default, the separator is '|'.\n");
        printf("  Example: ez-sp --sep ',' (uses a comma as a separator instead).\n");
    }
    else if (!strncmp(with, "style", 5))
    {
        printf("Usage of -s or --style:\n");
        printf("  Allows specifying a custom style for the output.\n");
        printf("  By default, no style is used.\n");
        printf("  Example: ez-sp --style my-style\n");
        printf("  Run ez-sp --help=styles to know more.\n");
    }
    else if (!strncmp(with, "debug", 5))
    {
        printf("Usage of -s or --debug:\n");
        printf("  Allows user to see what tokens were found.\n");
        printf("  By default, no debug info is shown.\n");
    }
    else if (!strncmp(with, "styles", 6))        { know_more_styles(); }
    else if (!strncmp(with, "formats", 7))       { know_more_formats(); }
    else if (!strncmp(with, "define-style", 12)) { know_more_define_style(); }
    else { usage_usage(NULL); }

    exit(EXIT_SUCCESS);
}

static void know_more_formats (void)
{
    printf("\n1. CSV\n");
    printf("   Example:\n");
    printf("   Name,Age,Score\n");
    printf("   Alice,25,90\n");
    printf("   Bob,30,85\n\n");

    printf("2. SSV (default style, here's where `sep` flag is used)\n");
    printf("   Example:\n");
    printf("   Name   | Age   | Score\n");
    printf("   Alice  | 25    | 90\n");
    printf("   Bob    | 30    | 85\n\n");

    printf("3. ASCII Table:\n");
    printf("   Example:\n");
    printf("   +--------+-----+-------+\n");
    printf("   | Name   | Age | Score |\n");
    printf("   +--------+-----+-------+\n");
    printf("   | Alice  |  25 |    90 |\n");
    printf("   | Bob    |  30 |    85 |\n");
    printf("   +--------+-----+-------+\n\n");

    printf("4. Markdown Table:\n");
    printf("   Example:\n");
    printf("   | Name  | Age | Score |\n");
    printf("   |-------|-----|-------|\n");
    printf("   | Alice |  25 |    90 |\n");
    printf("   | Bob   |  30 |    85 |\n\n");

    printf("5. JSON:\n");
    printf("   Example:\n");
    printf("   [\n");
    printf("     {\"Name\": \"Alice\", \"Age\": 25, \"Score\": 90},\n");
    printf("     {\"Name\": \"Bob\", \"Age\": 30, \"Score\": 85}\n");
    printf("   ]\n\n");

    printf("6. YAML:\n");
    printf("   Example:\n");
    printf("   - Name: Alice\n");
    printf("     Age: 25\n");
    printf("     Score: 90\n");
    printf("   - Name: Bob\n");
    printf("     Age: 30\n");
    printf("     Score: 85\n\n");

    printf("7. XML:\n");
    printf("   Example:\n");
    printf("   <table>\n");
    printf("     <row>\n");
    printf("       <Name>Alice</Name>\n");
    printf("       <Age>25</Age>\n");
    printf("       <Score>90</Score>\n");
    printf("     </row>\n");
    printf("     <row>\n");
    printf("       <Name>Bob</Name>\n");
    printf("       <Age>30</Age>\n");
    printf("       <Score>85</Score>\n");
    printf("     </row>\n");
    printf("   </table>\n\n");

    printf("8. HTML Table:\n");
    printf("   Example:\n");
    printf("   <table border=\"1\">\n");
    printf("     <tr>\n");
    printf("       <th>Name</th><th>Age</th><th>Score</th>\n");
    printf("     </tr>\n");
    printf("     <tr>\n");
    printf("       <td>Alice</td><td>25</td><td>90</td>\n");
    printf("     </tr>\n");
    printf("     <tr>\n");
    printf("       <td>Bob</td><td>30</td><td>85</td>\n");
    printf("     </tr>\n");
    printf("   </table>\n\n");
}

static void know_more_styles (void)
{
    printf("ez-sp: Conditional Formatting Options\n");
    printf("Only ANSI (terminal) and HTML outputs support text styling. Other formats remain raw.\n\n");

    printf("Supported Formatting Styles:\n");
    printf("  1. Foreground Color   - Changes text color (ANSI and HTML only)\n");
    printf("  2. Background Color   - Changes background color (ANSI and HTML only)\n");
    printf("  3. Bold               - Makes text bold (ANSI and HTML only)\n");
    printf("  4. Italic             - Italicizes text (ANSI and HTML only)\n");
    printf("  5. Strikethrough      - Adds a strikethrough effect (ANSI and HTML only)\n");
    printf("  6. Dim                - Reduces text intensity (ANSI and HTML only)\n");
    printf("  7. Blink              - Makes text blink (ANSI and HTML only, HTML deprecated)\n\n");

    printf("Example Usage:\n");
    printf("  - ANSI Output (SSV):\n");
    printf("      \033[31mRed Text\033[0m  (Foreground Red)\n");
    printf("      \033[1mBold Text\033[0m  (Bold)\n");
    printf("  - HTML Output:\n");
    printf("      <span style=\"color: red;\">Red Text</span>\n");
    printf("      <b>Bold Text</b>\n");
    printf("  - Others Output:\n");
    printf("      No styling is applied. Output remains in raw text format.\n\n");

    printf("To specify formatting, use the `--style` flag with ANSI or HTML outputs.\n");
    printf("To know how to define the styles run ez-sp --help=define-style\n\n");
}

static void know_more_define_style (void)
{
    printf("TODO\n");
}
