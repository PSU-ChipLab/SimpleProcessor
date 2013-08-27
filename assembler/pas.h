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
 */
#include <stdio.h>
#include <stdlib.h>

#include "chips/chip_v1.h"
#include "chips/chip_v0.h"

void err_ln(char *format, ...);

typedef enum
{
    SimpleV1,
    SimpleV2,
    Count  /* MUST BE LAST */
} SupportedChips;

extern void (*lineParse[Count])();
extern void (*lineParse2[Count])();
extern FILE *outStream;
extern FILE *errStream;
extern int exit_status;
extern int chipSelected;
extern const char *curFile;
extern size_t curLine;