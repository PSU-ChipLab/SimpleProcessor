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
 */

/*
 *  PAS - Penn State Assembler
 * 
 *      By David Schor
 *
 * 
 *  Supports Chips:
 * 
 *      - Version 1.0
 *      - Version 2.0
 * 
 * 
 *  ./pas [options] files...
 *
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include "pas.h"
#include "parser.h"
#include "symb.h"

void (*lineParse[Count])() =
{
  asm_v0_pass1,
  asm_v1_pass1
};
void (*lineParse2[Count])() =
{
  asm_v0_pass2,
  asm_v1_pass2
};

FILE *outStream = NULL;
FILE *errStream = NULL;
int exit_status = EXIT_SUCCESS;
int chipSelected = 0;
const char *curFile;
size_t curLine;

void setupOut();
void freeOut();

int main(int argc, char *argv[])
{
    int index; (void)argc, (void)argv;
    
    errStream = stderr;
    chipSelected = 0;
    
    index = 1;
    for (; argv[index]; ++index)
    {
        symb_init();
        inst_init();
        curFile = argv[index];
        setupOut();
        parse_file();
        if (exit_status == EXIT_SUCCESS)
             lineParse2[chipSelected]();
        freeOut();
        symb_free();
        inst_free();
    }
    
    return exit_status;
}


void setupOut()
{
    char *fname = malloc(strlen(curFile) + 5);
    
    strcpy(fname, curFile);
    strcat(fname, ".bin");
    
    if (!(outStream = fopen(fname, "w+")))
    {
        exit_status = 1;
        fprintf(errStream, "** PAS: Error opening file `%s': %s\n",
                curFile,
                strerror(errno));
        return;
    }
    
    free(fname);
}

void freeOut()
{
    fclose(outStream);
}

void err_ln(char *format, ...)
{
    va_list args;
    exit_status = 1;
    va_start(args, format);
    fprintf(errStream, "** PAS: %s:%lu: ", curFile, curLine);
    vfprintf(errStream, format, args);
    va_end(args);
}