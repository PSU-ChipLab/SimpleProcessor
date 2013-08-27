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

#include "../pas.h"
#include "../parser.h"
#include "../utils.h"
#include "../symb.h"
#include <string.h>
/*
 * Instruction set for the V27K-CT/CS/CU/... (our version 1)
 * 
 * By David Schor.
 * 
 * ===========================================================================
 * instructions:
 * ------------
 *  MV   -  Move data from source to destination
 *  ADD  -  Add data from source to destination and save the result in destination
 *  SUB  -  Subtract source from destination and save the result in destination
 *  BC   -  Branch if Carry bit is set
 *  IN   -  Move data from Input port to destination
 *  OUT  -  Move data from source to Output port
 * ===========================================================================
 * Op-code bit field: 
 * ------------------
 *      Bit     7       6       5       4       3       2       1       0
 *      Name    Branch  IO      AMOD    ARITH   SUB     NU      NU      NU
 * ===========================================================================
 * Op-code table:
 * --------------
 *  Inst   Bit 7  Bit 6  Bit 5  Bit 4  Bit 3  Bit 2  Bit 1  Bit 0   Addr. Mode
 *  MV #   0      0      1      0      0      0      0      0       Immediate
 *  MV     0      0      0      0      0      0      0      0       RR
 *  ADD    0      0      0      1      0      0      0      0       RR
 *  SUB    0      0      0      1      1      0      0      0       RR
 *  BC     1      0      0      0      0      0      0      0       RR
 *  IN     0      1      0      0      0      0      0      0       RR
 *  OUT    0      1      0      0      1      0      0      0       RR
 * ===========================================================================
 * ROM:
 * ----
 *         16-bit x 32
 * ===========================================================================
 * Possible Faults:
 * ----------------
 * Address 1 costs 1/4 duty cycle on reset. Unless some additional tests are
 * done, it should remain a NOP.
 * ===========================================================================
 */

/* There are 8 RAM registers */
#define MAX_REG 8
/* There are 32 ROM registers */
#define MAX_ROM 32

/* the current memory position */
static size_t at = 1;

/* the max amount of operands */
#define MAX_OPND 3

#define SRC 1 /* source */
#define DST 2 /* destination */
#define RLT 3 /* relative */
#define PSD 4 /* pseudo inst */
#define X   0 /* don't care */

/*
 *  
 * 
 */
struct opcode
{
    /* mnemonic */
    const char *op;
    /* argument count */
    size_t argc;
    /*
     * The typs specifies the operands types:
     *  
     *  # - immediate value
     *  $ - address value
     * 
     */
    const char *type;
    /* actual code */
    int code;
    /* oper type */
    int otype;
    struct
    {
        /* mnemonic */
        const char *op;
        /* type */
        const char *type;
        /* operators involved */
        int oprs[MAX_OPND];
        /* which operator to substitute */
        int loc[MAX_OPND];
    } ops[5];
} opcodes[] =
{
    /* Instructions */
    { "MV",  2, "$$",  0x00, X,   { { NULL, NULL, { 0 }, { 0 } } } },
    { "MV",  2, "#$",  0x04, X,   { { NULL, NULL, { 0 }, { 0 } } } },
    { "ADD", 2, "$$",  0x02, X,   { { NULL, NULL, { 0 }, { 0 } } } },
    { "SUB", 2, "$$",  0x03, X,   { { NULL, NULL, { 0 }, { 0 } } } },
    { "BC",  1, "$",   0x10, RLT, { { NULL, NULL, { 0 }, { 0 } } } },
    { "IN",  1, "$",   0x08, DST, { { NULL, NULL, { 0 }, { 0 } } } },
    { "OUT", 1, "$",   0x09, SRC, { { NULL, NULL, { 0 }, { 0 } } } },
    /*
     *  
     *  Pseudoinstruction
     *  
     */
    { "INC", 1, "$",   0x00, PSD, /* INC   X   */
        {
            { "MV",  "#$", { 0x01, 0x00 }, { -1, -1 } }, /* MV   #1, 0  */
            { "ADD", "$$", { 0x00, 0x00 }, { -1, 1 } },  /* ADD  0, X   */
            { NULL, NULL, { 0 }, { 0 } }
        }
    },
    { "DEC", 1, "$",   0x00, PSD, /* INC   X   */
        {
            { "MV",  "#$", { 0x01, 0x00 }, { -1, -1 } }, /* MV   #1, 0  */
            { "SUB", "$$", { 0x00, 0x00 }, { -1, 1 } },  /* SUB  0, X   */
            { NULL, NULL, { 0 }, { 0 } }
        }
    },
    { "DECZ", 1, "$",   0x00, PSD, /* DECZ   X   */
        {
            { "MV",  "#$", { 0xFF, 0x00 }, { -1, -1 } }, /* MV   #0xFF, 0  */
            { "ADD", "$$", { 0x00, 0x00 }, { -1, 1 } },  /* ADD  0, X   */
            { NULL, NULL, { 0 }, { 0 } }
        }
    },
    { "NOP", 0, "",   0x00, PSD, /* NOP */
        {
            { "MV",  "$$", { 0x00, 0x00 }, { -1, -1 } }, /* MV   0, 0  */
            { NULL, NULL, { 0 }, { 0 } }
        }
    },
    { "CLR", 1, "$",   0x00, PSD, /* CLR  X   */
        {
            { "MV",  "#$", { 0x00, 0x00 }, { -1, 1 } }, /* MV   #0, X  */
            { NULL, NULL, { 0 }, { 0 } }
        }
    },
    { "BLT", 3, "$$$",   0x00, PSD, /* BLT   X,Y,Z   */
        {
            { "MV",  "#$", { 0x01, 0x00 }, { -1, -1 } },  /* MV    #1, 0  */
            { "SUB", "$$", { 0x00, 0x00 }, { 2, 1   } },  /* SUB   Y, X   */
            { "BC",  "$",  { 0x00       }, { 3      } },  /* BC    Z   */
            { NULL, NULL, { 0 }, { 0 } }
        }
    },
    { "BGT", 3, "$$$",   0x00, PSD, /* BGT   X,Y,Z   */
        {
            { "MV",  "#$", { 0x01, 0x00 }, { -1, -1 } },  /* MV    #1, 0  */
            { "SUB", "$$", { 0x00, 0x00 }, { 2, 1   } },  /* SUB   X, Y   */
            { "BC",  "$",  { 0x00       }, { 3      } },  /* BC    Z   */
            { NULL, NULL, { 0 }, { 0 } }
        }
    },
    { "BNZ", 2, "$",   0x00, PSD, /* BNZ   X, Y   */
        {
            { "MV",  "#$", { 0x01, 0x00 }, { -1, -1 } },  /* MV   #0xFF, 0  */
            { "ADD", "$$", { 0x00, 0x00 }, { -1, 1  } },  /* ADD  0, X   */
            { "BC",  "$",  { 0x00       }, {  2     } },  /* BC   Y    */
            { NULL, NULL, { 0 }, { 0 } }
        }
    },
    { NULL,  0, NULL,  0x00, X,   { { NULL, NULL, { 0 }, { 0 } } } }
};

#define errln(s) do { err_ln(s); return; } while (0)
#define errln2(s,x) do { err_ln(s,x); return; } while (0)

/*
 * Returns the number of operands a given mnemonic has.
 * As it stand we are assuming, regardless of the type of
 * operands, they will always have the same amount of operands.
 * This assumptions makes it easier since we don't need to
 * perform a single look a head check to see the operand type.
 */
int getOpCount(const char *mnemonic)
{
    size_t i;
    
    for (i = 0; opcodes[i].op; ++i)
        if (!strcmpi(opcodes[i].op, mnemonic))
            return opcodes[i].argc;
        
    return -1;
}
/*
 * Check if the arguments give up until this point are actually
 * valid.
 */
int isOperandLegal(const char *mnemonic, const char *type)
{
    size_t i;
    
    for (i = 0; opcodes[i].op; ++i)
        if (!strcmpi(opcodes[i].op, mnemonic))
            if (!strncmp(opcodes[i].type, type, strlen(type)))
                return 1;
        
    return 0;
}
/*
 * 
 */
struct opcode *getInst(const char *mnemonic, const char *type)
{
    size_t i;
    
    for (i = 0; opcodes[i].op; ++i)
        if (!strcmpi(opcodes[i].op, mnemonic))
            if (!strcmp(opcodes[i].type, type))
                return opcodes + i;
            
    return NULL;
}
/*
 * Get the amount of instructions the mnemonic costs
 */
int getInstMem(const char *mnemonic, const char *type)
{
    int i = 1;
    struct opcode *s = getInst(mnemonic, type);
    
    if (s->otype == PSD)
        for (i = 0; s->ops[i].op; ++i);
        
    return i;
}

/*
 * Parse a single line of instruction
 * 
 * 
 * Here we do a few things:
 * 
 *      - We make sure the syntax is valid
 *      - We store equate statements
 *      - We store label positions
 *      - We store parsed instructions for pass 2.
 *      - We make sure the operands make sense
 *      - We make sure the program fits in ROM
 */
void asm_v0_pass1(const char *line)
{
    const char *original = line;
    char *inst, type[MAX_OPND+1];
    char *opers[MAX_OPND+1];
    int argc, i;
    lexeme l = parse_next(&line);
    
    /*
     * First dump the label, if any.
     * 
     */
    if (isLabel(l))
    {
        const char *temp = getLabel(l);
        
        /* fetch the next token */
        l = parse_next(&line);
        
        /*
         * Check if it's an equate statement.
         */
        if (isTag(l) && !strcmpi(getTag(l), "equ"))
        {
            free(getTag(l)); /* free "equ" */
            
            /* fetch next token */
            l = parse_next(&line);
            
            if (isEOL(l))
                errln("Missing token on equate statement.\n");
            if (!isTag(l))
                errln("Unexpected token on equate statement.\n");
            
            symb_push_const(temp, getTag(l));
           
            /* just make sure there is nothing else on this line */
            l = parse_next(&line);
            
            if (!isEOL(l))
                errln("Unexpected token on equate statement.\n");
            
            return;
        }
        /* It's just a label */
        else
            symb_push_label(temp, at);
    
        /* fetch next token */
        /*l = parse_next(&line);*/
    }
    
    /* eol? bail out */
    if (isEOL(l))
        return;
    
    /* if we made it here, it must be a mnemonic */
    if (!isTag(l))
        errln("Unexpected token. Expecting mnemonic.\n");
    
    /* get the arguments for the statement */
    inst = getTag(l);
    argc = getOpCount(inst);
    
    
    /* check if the mneominc even exist s */
    if (argc < 0)
        errln2("The mnemonic `%s` is unrecognized.\n", inst);

    /* parse each of the operands */
    type[0] = '\0';
    for (i = 0; i < argc; ++i)
    {
        /* fetch next token */
        l = parse_next(&line);
        
        if (isEOL(l))
            errln2("Missing operands. Expecting %d operands.\n", argc);
        
        /* append the operand type */
        type[i] = isTag(l) ? '$' : '#';
        type[i+1] = '\0';
        
        if (!isOperandLegal(inst, type))
            errln2("Operand %d is of invalid type.\n", i+1);
        
        opers[i] = isTag(l) ? getTag(l) : getImmed(l);
    }
        
    
    /* fetch next token */
    l = parse_next(&line);
    
    /* there shouldn't be any tokens */
    if (!isEOL(l))
        errln2("Too many operands. Expecting %d operands.\n", argc);
    
    /* null-terminate the oper-list */
    opers[argc] = NULL;
    
    /* push the instruction for phase 2 */
    inst_push2(at, original, inst, 0, 0,
               (const void*)getInst(inst, type), (const char**)opers);
    
    /*
     * Make sure there is enough memory
     */
    at += getInstMem(inst, type);/*
    printf("%s = %u (%u)\n", inst, getInstMem(inst, type), at);*/
    if (at > MAX_ROM)
        errln2("ROM size exceeded. Limit = %u\n", MAX_ROM);
}



/*
 *  Evalute the constant until we can no longer
 *  evalute it.
 * 
 *  Has a 100 recursive scans limit to prevent infinite
 *  recursive tags.
 * 
 */
int getConstVal(const char *str, long *val, int *wasLabel)
{
    int recuse = 0;
    const char *temp = str;
    
    if (wasLabel)
        *wasLabel = 0;
    
    /* check if it's a value or a tag; allowed recursive definitions */
    while (!isnum(temp, val) && symb_get_const(temp))
    {
        /* prevent infinite "recusive" tags */
        if (recuse > 100)
        {
            exit_status = 1;
            fprintf(errStream, "** PAS: %s: `%s' definition leads to an infinite recusion.\n",
                    curFile, temp);
            return 0;
        }
        temp = symb_get_const(temp);
        recuse++;
    }
    
    /* if we hit a wall here, let's try the labels */
    if (!isnum(temp, val))
    {
        size_t index;
        /*
         *  Attempt to find a label.
         */
        if ((index = symb_get_label(temp)) != -1U)
        {
            *val = (int)index;
            if (wasLabel)
                *wasLabel = 1;
        }
        else
        {
            exit_status = 1;
            fprintf(errStream, "** PAS: %s: `%s' definition does not yield a value.\n",
                    curFile, temp);
            return 0;
        }
    }
    
    return 1;
}


void gen(unsigned char op, unsigned char src, unsigned char dest)
{
    /*
     *  INST <4:0> | SRC <7:0> | DEST <2:0>
     *       5            8        3         = 16-bits
     *  [][][][][] [][][] | [][][][][] [][][]
     *          HI                 LO
     */
    unsigned char hi = (op << 3) | (src >> 5);
    unsigned char lo = (src << 3) | (dest & 0x07);
    printf(": %d,%d,%d :%d %d\n", op, src, dest, hi, lo);
    fputc(hi, outStream);
    fputc(lo, outStream);
}

void igen(unsigned char o, unsigned char a, int t)
{
    /* one operand mne only take memory addresses */
    if (t == SRC || t == RLT)
        gen(o, a, 0x00); /* destination => 0 */
    else
        gen(o, 0x00, a); /* source  => 0 */
}

void pgen(size_t loc, struct opcode *o, char **opers)
{
    size_t i;
    
    for (i = 0; o->ops[i].op; ++i)
    {
        int l = 0;
        struct opcode *inst = getInst(o->ops[i].op, o->ops[i].type);
        long a, b;
        
        /* first operand */
        if (o->ops[i].loc[0] == -1)
            a = o->ops[i].oprs[0];
        else
            getConstVal(opers[o->ops[i].loc[0] - 1], &a, &l);
        
        /* second operand */
        if (o->ops[i].loc[1] == -1)
            b = o->ops[i].oprs[1];
        else
            getConstVal(opers[o->ops[i].loc[1] - 1], &b, &l);

        /* [Label Location] - ([This Line] + 2) */
        if (inst->otype == RLT && l)
            a = a - (loc + 2);
        
        /*printf("=> %s\n", inst->op);
        */
        gen(inst->code, a, b);
    }
}

/*
 * Parse a single line of instruction
 */
void asm_v0_pass2()
{
    long a, b;
    size_t i, loc, count;
    
    /* first one is a NOP */
    gen(0x00, 0x00, 0x00);
    
    count = inst_count();
    
    for (i = 0, loc = 0; i < count; ++i)
    {
        size_t at;
        const char *line;
        const char *inst;
        long data1, data2;
        struct opcode *o;
        size_t argcount;
        char *opers[MAX_OPND+1];

        /* get the data */
        inst_get2(i, &at, &line, &inst, &data1, &data2, (const void **) &o, &argcount, (const char **)opers);
        
        /* The most basic setup */
        if (o->otype == X
            && getConstVal(opers[0], &a, NULL)
            && getConstVal(opers[1], &b, NULL))
            gen(o->code, a, b);
        else if (o->otype == PSD)
            pgen(loc, o, opers);
        else if (getConstVal(opers[0], &a, NULL))
        {
            int l;

            /* [Label Location] - ([This Line] + 2) */
            if (o->otype == RLT && getConstVal(opers[0], &a, &l) && l)
                a = a - (loc + 2);
            
            igen(o->code, a, o->otype);
        }
        
        loc += getInstMem(o->op, o->type);
    }
}
