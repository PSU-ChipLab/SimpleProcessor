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
 * 
 * 
 * Functions to work with symbols and instructions.
 * 
 * These functions are somewhat abstracted from the actual
 * instructions in order to allow us to expand to additional
 * chipsets.
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * ==========================================================
 * = Symbol Table
 * =
 * = The symbol table is a very generic table that is capable
 * = of holding an arbitrary amount of of lables/offsets and
 * = val/key pairs.
 * =
 * = The table is currently implemented as a simple array,
 * = which is not very efficient but it works just fine since
 * = we are working with limited ROM anyway.
 * =
 * = Todo: Perhaps switch to a hash table.
 * =
 * =
 * 
 * Setup the symbol table.
 * 
 * If called again, it resets the table back to zero - this
 * does free all its content in the process.
 */
void symb_init(void);
/*
 * Frees a previously initialized symbol table. This frees all
 * of it's contents as well.
 */
void symb_free(void);
/*
 * Push a key/val pair onto the symbol table.
 * 
 * Note: Currently pushing an key/val with a key that already
 *       exists will cause the old key/val pair to be freed;
 * 
 * Note: BOTH the key and val MUST been dynamically allocated.
 */
void symb_push_const(const char *key, const char *val);
/*
 * Push a label with its index onto the symbol table.
 * 
 * Note: Currently pushing an label/index with a key that already
 *       exists will cause the old label to be freed;
 * 
 * Note: The key MUST been dynamically allocated.
 */
void symb_push_label(const char *label, size_t index);
/*
 * Returns the value associated with the specified key.
 * 
 * Note: If the key does not exist, NULL is returned.
 */
const char *symb_get_const(const char *key);
/*
 * Returns the index associated with specified label.
 * 
 * Note: If the values does not exist, -1U is returned.
 */
size_t symb_get_label(const char *label);
/*
 * ==========================================================
 * = Instruction Table
 * =
 * = The symbol table is a very generic table that is capable
 * = of holding an arbitrary amount of of instructions+data.
 * =
 * = The table is currently implemented as a simple array,
 * = which is not very efficient but it works just fine since
 * = we are working with limited ROM anyway.
 * =
 * = Todo: Perhaps switch to a hash table.
 * =
 * =
 * 
 */
/*
 * Setup the instruction table.
 * 
 * If called again, it resets the table back to zero - this
 * does free all its content in the process.
 */
void inst_init(void);
/*
 * Frees a previously initialized symbol table. This frees all
 * of it's contents as well.
 */
void inst_free(void);
/*
 * Get the instruction table size.
 */
size_t inst_count();
/*
 * Push an instruction along with some data associated with it for
 * later retrival. All arguments are optional - for the actual
 * implementation to be used, take advantage of it.
 */
void inst_push(
    size_t line_num,    /* the line number of the instruction */
    const char *line,   /* the actual line of code */
    const char *inst,   /* the instruction associated with this line */
    const long data1,   /* store some data associated with the instruction */
    const long data2,   /* store some data associated with the instruction */
    const void *data3,  /* store some data associated with the instruction */
    ...                 /* a null-terminated list of operands, `const char*` */
);
void inst_push2(
    size_t line_num,    /* the line number of the instruction */
    const char *line,   /* the actual line of code */
    const char *inst,   /* the instruction associated with this line */
    const long data1,   /* store some data associated with the instruction */
    const long data2,   /* store some data associated with the instruction */
    const void *data3,  /* store some data associated with the instruction */
    const char **oprs   /* a null-terminated list of operands, `const char*` */
);
/*
 * Retrive the at'th instruction from the table along with all
 * the data associated with it.
 */
void inst_get(
    size_t at,          /* the index of the instruction to get */
    size_t *line_num,   /* the line number of the instruction */
    const char **line,  /* the actual line of code */
    const char **inst,  /* the instruction associated with this line */
    long *data1,        /* some data associated with the instruction */
    long *data2,        /* some data associated with the instruction */
    const void **data3, /* some data associated with the instruction */
    size_t *opercount,  /* the number operands originally stored */
    ...                 /* a list of operands, `const char**` */
);
void inst_get2(
    size_t at,          /* the index of the instruction to get */
    size_t *line_num,   /* the line number of the instruction */
    const char **line,  /* the actual line of code */
    const char **inst,  /* the instruction associated with this line */
    long *data1,        /* some data associated with the instruction */
    long *data2,        /* some data associated with the instruction */
    const void **data3, /* some data associated with the instruction */
    size_t *opercount,  /* the number operands originally stored */
    const char **oprs   /* a list of operands, `const char**` */
);
void inst_push_inst(int curLine, const char *line, const char *inst, int type, const char *op1, const char *op2);
void inst_get_inst(size_t at, int *curLine, const char **line, const char **inst, int *type, const char **op1, const char **op2);

