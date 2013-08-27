#include <stddef.h>
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
 * Some generic functions to perform some common operations - mainly
 * string ones.
 * 
 * 
 * 
 * 
 * 
 * 
 * A simple routine to compare in a case-insensitive manner
 */
int strcmpi(const char *s1, const char *s2);
/*
 * Duplicate a string up to N characters.
 */
char *strndup(const char *str, size_t n);
/*
 * Duplicate a string.
 */
char *strdup(const char *str);
/*
 * 
 */
/*
 * Determins if the number can be parsed as a number.
 * Stores the value in num at the same time.
 * 
 * Some things it considers:
 * 
 *      - Hex:  0xNNNNNN
 *      - Bin:  0bNNNNNN
 *      - Oct:  0NNNNNNN
 *      - Dec:  NNNNNNNN
 *      - Chr:  'X'
 */
int isnum(const char *str, long int *num);
/*
 * 
 */
/* EOF */