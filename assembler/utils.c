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

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
/*
 * Perform a single case-insensitive comparsion
 */
int strcmpi(const char *s1, const char *s2)
{
    for ( ; tolower(*s1) == tolower(*s2); s1++, s2++)
        if (*s1 == '\0')
            return 0;
    return tolower(*s1) - tolower(*s2);
}
/*
 * Duplicate a string up to N characters.
 */
char *strndup(const char *str, size_t n)
{
    char *r;

    if ((r = malloc(n + 1)))
    {
        r[n] = '\0';
        memcpy(r, str, n);
    }
    return r;
}
/*
 * Duplicate a string.
 */
char *strdup(const char *str)
{
    return strndup(str, strlen(str));
}
/*
 * convert a number to hex
 */
int isnum(const char *str, long *num)
{
    char *temp;
    int base;
    
    /* hex */
    if (*str == '0' && tolower(str[1]) == 'x')
        base = 16, str += 2;
    /* binary */
    else if (*str == '0' && tolower(str[1]) == 'b')
        base = 2, str += 2;
    /* octal */
    else if (*str == '0')
        base = 8, str += 1;
    /* decimal */
    else
        base = 10;
    
    /* check for a possible char literal */
    if (*str == '\''
        && isalnum(str[1])
        && str[2] == '\''
        && str[3] == '\0')
    {
        *num = (long)str[1];
        return 1;
    }
    
    *num = strtol(str, &temp, base);
    return *temp == '\0';
} 
