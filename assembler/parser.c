#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "pas.h"
#include "parser.h"
#include "utils.h"

#define MAX_LINE 1024

extern int exit_status;


void parse_file()
{
    FILE *f;
    int c;
    size_t at;
    char buff[MAX_LINE+1];
    
    if (!(f = fopen(curFile, "r")))
    {
        exit_status = 1;
        fprintf(errStream, "** PAS: Error opening file `%s': %s\n",
                curFile,
                strerror(errno));
        return;
    }
    
    /* accumulate lines and send them to be parsed */
    for (at = 0, curLine = 0; (c = getc(f)) != EOF; ++at)
    {
        if (c == '\n')
        {
            buff[at] = '\0';
            at = -1;
            ++curLine;
            /* parse the line, this is chip-specific */
            lineParse[chipSelected](strdup(buff));
            continue;
        }
        
        if (at >= MAX_LINE)
        {
            exit_status = 1;
            fprintf(errStream, "** PAS: %s:%lu: Line reached line limit.\n",
                    curFile, curLine);
            /* consume the rest of the line */
            while ((c = getc(f)) != EOF && c != '\n');
            at = -1;
            ++curLine;
            continue;
        }
        
        buff[at] = c;
    }
    
    
    if (at > 0)
    {
        buff[at] = '\0';
        lineParse[chipSelected](strdup(buff));
    }
    
    
    fclose(f);
}

/* 
 * This fucntion eats up the whitespace.
 */
static void eat_ws(const char **line)
{
    /* first consume whitespace */
    while (**line == '\t' || **line == ' ')
        (*line)++;
}

lexeme parse_next(const char **line)
{
    const char *start;
    lexeme l;
    
    /* eat whitespace */
    eat_ws(line);
    
    /* start of line */
    start = *line;
    
    /* eol or comments, we are done */
    if (!**line || **line == ';')
    {
        l.type = eol;
        return l;
    }
    
    /* consume up to the matching delimiter */
    while (**line && !strchr(" \t:,;", **line))
        (*line)++;
    
    l.type = tag;
    l.val.str = strndup(start + (*start == '#'), *line - start - (*start == '#'));
    
    /* eat whitespace */
    eat_ws(line);
    
    switch (**line)
    {
        case ',':
            *line += 1;
            l.type = tag;
            break;
        case ':':
            *line += 1;
            l.type = label;
            break;
    }
    
    /* #* is an immediate */
    if (l.type == tag && *start == '#')
        l.type = immediate;
    
    return l;
}