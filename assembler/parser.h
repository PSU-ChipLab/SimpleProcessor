/*
 * Copyright (C) 2007,2008,2009,2010,2011,2012,2013 David Schor
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall
 * be included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 * IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 * 
 * 
 * 
 *  This is the parser generic functions used
 *  mostly by the asm generator of the specific
 *  chip.
 * 
 * 
 * 
 * 
 * 
 *
 * 
 * 
 * 
 */
/* 
 * A struct that holds a single token.
 */
typedef struct lexeme lexeme;
/*
 * Opens the specific file (curFile) and reads it
 * line by line. Lines are sent to the specific chip
 * asm generator.
 */
void parse_file(void);
/*
 * Returns the new allocated string with the
 * next token in the line. NULL if the end of
 * the line is reached.
 */
lexeme parse_next(const char **line);
/*
 * 
 */
#define isEOL(l) ((l).type == eol)
#define isTag(l) ((l).type == tag)
#define isLabel(l) ((l).type == label)
#define isImmed(l) ((l).type == immediate)
#define getTag(l) ((l).val.str)
#define getLabel(l) ((l).val.str)
#define getImmed(l) ((l).val.str)
struct lexeme
{
    enum
    {
        /* a label like "foobar:", value will not have the colon */
        label,
        /* a value prefixed with '#' */
        immediate,
        /* a tag, which could be a directive or a label or an operand */
        tag,
        /* end of line, comments are also considered EOL */
        eol
    } type;
    union
    {
        int val;
        char *str;
    } val;
};
/*
 * 
 */
/* EOF */