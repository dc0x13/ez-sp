#ifndef ARGXS_ARGXS_H
#define ARGXS_ARGXS_H

/* Defines if a flag does take an argument, for example
 * if the -d flag is defined as ARGXS_ARG_YES, then its
 * usage must be -d "argument", else argxs will detect
 * it as an error
 */
#define ARGXS_ARG_NON 0
#define ARGXS_ARG_YES 1
#define ARGXS_ARG_MAY 2

/* This must be always included within the array of flags
 * since this works as a the delimiter
 */
#define ARGXS_FINAL_FLAG    {NULL, 0, 0}

/* none:       do not worry
 * non_sense:  the current argv element does not make quite sense
 * undef_flag: unknonwn flag found
 * */
enum argxs_fatal
{
    argxs_fatal_none,
    argxs_fatal_non_sense,
    argxs_fatal_undef_flag,
    argxs_fatal_unnecessary_arg,
    argxs_fatal_arg_expected
};

/* flagname: a long name for the flag (--document)
 * id:       simple character representation (-D)
 * q_arg:    argument option
 */
struct argxs_flag
{
    char          *flagname;
    char          id;
    unsigned char q_arg;
};

/* flag:      pointer to the original flag
 * argument:  given argument (if any)
 */
struct argxs_found
{
    struct        argxs_flag *flag;
    char          *argument;
};

/* p_args:    positional arguments
 * found:     found flags
 * last:      last flag used (in case of error)
 * argc:      position within argv
 * no_found:  number of flags found
 * no_p_args: number of positional arguments
 * fatal:     cause of error (if any)
 */
struct argxs_res
{
    char         **p_args;
    struct       argxs_found *found;
    struct       argxs_found *last;
    unsigned int argc;
    unsigned int no_found;
    unsigned int no_p_args;
    enum         argxs_fatal fatal;
};

extern const char *const argxs_errors[5];

struct argxs_res *argxs_parse (const unsigned int, char**, const struct argxs_flag*);
void argxs_clean (struct argxs_res*);

#endif
