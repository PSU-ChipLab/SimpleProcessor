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

#include <stdlib.h>
#include <stdarg.h>
#include "symb.h"
#include "utils.h"

#define MAX_MEM 1024

typedef struct symb symb;
struct symb
{
    enum
    {
        sym_const,
        sym_label
    } type;
    const char *key;
    union
    {
        int value;
        const char *str;
    } val;
};

typedef struct inst inst;
struct inst
{
    int curLine;
    const char *line;
    const char *inst;
    int type;
    const char *op1;
    const char *op2;
};

typedef struct instruction instruction;
struct instruction
{
    int line_num;
    const char *line;
    const char *inst;
    long data1;
    long data2;
    const void *data3;
    size_t oper_count;
    const char **opers;
};

static symb *symbols;
size_t tbl_size;
size_t mem_size;

static inst *instructions;
size_t inst_tbl_itm_size;
size_t inst_tbl_mem_size;

static instruction *insts;
size_t inst_tbl_size;
size_t inst_tbl_mem;

void symb_init()
{
    if (!symbols)
    {
        mem_size = 100;
        tbl_size = 0;
        symbols = malloc(mem_size * sizeof (struct symb));
    }
    else
    {
        tbl_size = 0;
    }
}

void inst_init()
{
    if (!instructions)
    {
        inst_tbl_mem_size = 100;
        inst_tbl_itm_size = 0;
        instructions = malloc(inst_tbl_mem_size * sizeof (struct inst));
        
        inst_tbl_mem = 100;
        inst_tbl_size = 0;
        insts = malloc(inst_tbl_mem * sizeof (instruction));
    }
    else
    {
        inst_tbl_itm_size = 0;
        
        inst_tbl_size = 0;
    }
}

static void ensure_mem()
{
    if (tbl_size+1 > mem_size)
    {
        mem_size *= 2;
        symbols = realloc(symbols, mem_size * sizeof (symb));
    }
}

static void ensure_mem2()
{
    if (inst_tbl_itm_size+1 > inst_tbl_mem_size)
    {
        inst_tbl_mem_size *= 2;
        instructions = realloc(instructions, inst_tbl_mem_size * sizeof (struct inst));
    }

    if (inst_tbl_size+1 > inst_tbl_mem)
    {
        inst_tbl_mem *= 2;
        insts = realloc(insts, inst_tbl_mem * sizeof (instruction));
    }
}

void inst_free()
{
    size_t i;
    
    for (i = 0; i < inst_tbl_itm_size; ++i)
    {
        free((char*)instructions[i].line);
        free((char*)instructions[i].inst);
        free((char*)instructions[i].op1);
        free((char*)instructions[i].op2);
    }
    free(instructions);
    
    /*
    for (i = 0; i < inst_tbl_size; ++i)
    {
        free((char*)insts[i].line);
        free((char*)insts[i].inst);
    }
    free(insts);
    */
}

void symb_free()
{
    size_t i;
    
    for (i = 0; i < tbl_size; ++i)
    {
        if (symbols[i].type == sym_const)
            free((char*)symbols[i].val.str);
        free((char*)symbols[i].key);
    }
    free(symbols);
}

void symb_push_label(const char *label, size_t index)
{
    ensure_mem();
    symbols[tbl_size].type = sym_label;
    symbols[tbl_size].key = label;
    symbols[tbl_size++].val.value = index;
}

void symb_push_const(const char *key, const char *val)
{
    ensure_mem();
    symbols[tbl_size].type = sym_const;
    symbols[tbl_size].key = key;
    symbols[tbl_size++].val.str = val;
}

size_t inst_count()
{
    return inst_tbl_size;
}


void inst_push_inst(int curLine, const char *line, const char *inst, int type, const char *op1, const char *op2)
{
    ensure_mem2();
    instructions[inst_tbl_itm_size].curLine = curLine;
    instructions[inst_tbl_itm_size].line = line;
    instructions[inst_tbl_itm_size].inst = inst;
    instructions[inst_tbl_itm_size].type = type;
    instructions[inst_tbl_itm_size].op1 = op1;
    instructions[inst_tbl_itm_size++].op2 = op2;
}


void inst_get_inst(size_t at, int *curLine, const char **line, const char **inst, int *type, const char **op1, const char **op2)
{
    *curLine = instructions[at].curLine;
    *line = instructions[at].line;
    *inst = instructions[at].inst;
    *type = instructions[at].type;
    *op1 = instructions[at].op1;
    *op2 = instructions[at].op2;
}

const char *symb_get_const(const char *key)
{
    size_t i;
    for (i = 0; i < tbl_size; ++i)
        if (symbols[i].type == sym_const && !strcmpi(symbols[i].key, key))
            return symbols[i].val.str;
    return NULL;
}

size_t symb_get_label(const char *label)
{
    size_t i;
    for (i = 0; i < tbl_size; ++i)
        if (symbols[i].type == sym_label && !strcmpi(symbols[i].key, label))
            return symbols[i].val.value;
    return -1U;
}

/*
 * Push an instruction
 */
void inst_push(
    size_t line_num,    /* the line number of the instruction */
    const char *line,   /* the actual line of code */
    const char *inst,   /* the instruction associated with this line */
    const long data1,   /* store some data associated with the instruction */
    const long data2,   /* store some data associated with the instruction */
    const void *data3,  /* store some data associated with the instruction */
    ...                 /* a null-terminated list of operands, `const char*` */
)
{
    size_t opcount;
    const char *op;
    va_list args;
    
    insts[inst_tbl_size].line_num = line_num;
    insts[inst_tbl_size].line = line;
    insts[inst_tbl_size].inst = inst;
    insts[inst_tbl_size].data1 = data1;
    insts[inst_tbl_size].data2 = data2;
    insts[inst_tbl_size].data3 = data3;
    
    va_start(args, data3);
    for (opcount = 0; (op = va_arg(args, char*)); ++opcount);
    va_end(args);
    
    insts[inst_tbl_size].oper_count = opcount;
    insts[inst_tbl_size].opers = malloc(opcount * sizeof (char*));
    
    va_start(args, data3);
    for (opcount = 0; (op = va_arg(args, char*)); ++opcount)
        insts[inst_tbl_size].opers[opcount] = op;
    va_end(args);
    
    ++inst_tbl_size;
}
void inst_push2(
    size_t line_num,    /* the line number of the instruction */
    const char *line,   /* the actual line of code */
    const char *inst,   /* the instruction associated with this line */
    const long data1,   /* store some data associated with the instruction */
    const long data2,   /* store some data associated with the instruction */
    const void *data3,  /* store some data associated with the instruction */
    const char **oprs   /* a null-terminated list of operands, `const char*` */
)
{
    size_t opcount;
    
    insts[inst_tbl_size].line_num = line_num;
    insts[inst_tbl_size].line = line;
    insts[inst_tbl_size].inst = inst;
    insts[inst_tbl_size].data1 = data1;
    insts[inst_tbl_size].data2 = data2;
    insts[inst_tbl_size].data3 = data3;
    
    for (opcount = 0; oprs[opcount]; ++opcount);
    
    insts[inst_tbl_size].oper_count = opcount;
    insts[inst_tbl_size].opers = malloc(opcount * sizeof (char*));
    
    for (opcount = 0; oprs[opcount]; ++opcount)
        insts[inst_tbl_size].opers[opcount] = oprs[opcount];

    ++inst_tbl_size;
}
/*
 * Retrive the at'th instruction from the table along with all
 * the data associated with it.
 */
void inst_get(
    size_t at,          /* the index of the instruction to get */
    size_t *line_num,   /* the line number of the instruction */
    const char **line,  /* the actual line of code */
    const char **inst,  /* the instruction associated with this line */
    long *data1,        /* store some data associated with the instruction */
    long *data2,        /* store some data associated with the instruction */
    const void **data3, /* store some data associated with the instruction */
    size_t *opercount,  /* the number operands originally stored */
    ...                 /* a list of operands, `const char**` */
)
{
    size_t opcount;
    va_list args;
    
    *line_num = insts[at].line_num;
    *line = insts[at].line;
    *inst = insts[at].inst;
    *data1 = insts[at].data1;
    *data2 = insts[at].data2;
    *data3 = insts[at].data3;
    *opercount = opcount = insts[at].oper_count;
    
    va_start(args, opercount);
    for (opcount = 0; opcount < *opercount; ++opcount)
        *(char**)va_arg(args, char*) = (char*)insts[at].opers[opcount];
    va_end(args);
}

/*
 * Retrive the at'th instruction from the table along with all
 * the data associated with it.
 */
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
)
{
    size_t opcount;
    
    *line_num = insts[at].line_num;
    *line = insts[at].line;
    *inst = insts[at].inst;
    *data1 = insts[at].data1;
    *data2 = insts[at].data2;
    *data3 = insts[at].data3;
    *opercount = opcount = insts[at].oper_count;
    
    for (opcount = 0; opcount < *opercount; ++opcount)
        oprs[opcount] = (char*)insts[at].opers[opcount];
}