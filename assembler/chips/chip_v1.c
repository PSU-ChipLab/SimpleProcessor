/*
 * Copyright (C) 2013 David Schor
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
 */

#include <stdio.h>
#include <stdlib.h>
#include "chip_v1.h"
#include "../pas.h"
#include "../parser.h"

void asm_v1_pass1(const char *line)
{
    lexeme l;
    
    printf("Line: `%s'\n", line);
    
    while ((l = parse_next(&line)).type != eol)
    {
        switch (l.type)
        {
            case label:
                printf("   LABEL: `%s'\n", l.val.str);
                free(l.val.str);
                break;
            case tag:
                printf("   TAG: `%s'\n", l.val.str);
                free(l.val.str);
                break;
            case immediate:
                printf("   IMMED: `%s'\n", l.val.str);
                free(l.val.str);
                break;
            default:
                exit_status = 1;
                fprintf(errStream, "** PAS: %s:%lu: Unexpected token reached.\n",
                        curFile, curLine);
                return;
        }
    }
}


void asm_v1_pass2()
{
    /* NOP */
}