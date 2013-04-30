/*
 * Copyright (C) 2013 David Schor
 * 
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * 
 * 
 * 
 * 
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * 
 * Penn State Simple Processor Program Loader
 * 
 *  Supported Chips:
 * 
 *      - Version 0
 * 
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * 
 * Usage:
 * 
 *  ./loader [options] files...
 * 
 * 
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

static const char code_top[] =
{
    ";   Title:  Memory Loader\n"
    ";\n"
    "; Updated:  2/6/2013\n"
    ";\n"
    ";  Author:  David Schor\n"
    ";\n"
    "; Company:  The Pennsylvania State University\n"
    ";           Department of Computer Science and Engineering\n"
    ";           Chip Lab\n"
    ";\n"
    ";    Desc:  This is a ROM loader for the Simple Chip V0.\n"
    ";           This program is usually used with the ROM loader generator\n"
    ";           which is used to generate the data for this program.\n"
    ";\n"
    ";    Data:  The data to be written MUST be place between DataStart\n"
    ";           and DataEnd. 1 byte address followed by 2 bytes of data.\n"
    ";\n"
    "; PORTS:\n"
    ";\n"
    ";   Data:   8-bit HI  8-bit LO    \n"
    ";           [A07-A00][B07-B00]\n"
    ";\n"
    ";   Addr:   8-bit\n"
    ";           [P1-1P0][M05-M00]\n"
    ";\n"
    ";    Wrt:   1-bit\n"
    ";           [E4]\n"
    ";\n"
    ";\n"
    "; C128 Board Pins:\n"
    ";\n"
    ";\n"
    ";\n"
    ";   D00-D15 = 16-bit Data Port\n"
    ";   A00-A07 = 8-bit Address Port\n"
    ";   W0      = 1-bit Write Enable\n"
    ";   \n"
    ";                           ___________\n"
    ";                           | 01 | 02 |\n"
    ";                           | 03 | 04 |    \n"
    ";                           | 05 | 06 |\n"
    ";                     A07   | 07 | 08 |\n"
    ";                     A06   | 09 | 10 |\n"
    ";                           | 11 | 12 |\n"
    ";                           | 13 | 14 |\n"
    ";                           | 15 | 16 |\n"
    ";                     A04   | 17 | 18 |\n"
    ";                     A02   | 19 | 20 |\n"
    ";                     A05   | 21 | 22 |\n"
    ";                     A03   | 23 | 24 |\n"
    ";             1 -->   D15   | 25 | 26 |\n"
    ";             6       D14   | 27 | 28 |\n"
    ";             -       D13   | 29 | 30 |\n"
    ";             B       D12   | 31 | 32 |\n"
    ";             I       D11   | 33 | 34 |\n"
    ";             T       D10   | 35 | 36 |\n"
    ";                     D09   | 37 | 38 |\n"
    ";             D       D08   | 39 | 40 |\n"
    ";             A       D07   | 41 | 42 |\n"
    ";             T       D06   | 43 | 44 |\n"
    ";             A       D05   | 45 | 46 |\n"
    ";                     D04   | 47 | 48 |   W0 <-- Write Enable\n"
    ";             P       D03   | 49 | 50 |\n"
    ";             O       D02   | 51 | 52 |\n"
    ";             R       D01   | 53 | 54 |\n"
    ";             T -->   D00   | 55 | 56 |\n"
    ";                     A01   | 57 | 58 |\n"
    ";                     A00   | 59 | 60 |\n"
    ";                           -----------\n"
    ";\n"
    ";\n"
    ";\n"
    "; *\n"
    "; ******************************************************************\n"
    "; *\n"
    ";            XDEF        _start        ; export '_start' symbol\n"
    ";            ABSENTRY    _start        ; for assembly entry point\n"
    "; *\n"
    "; ******************************************************************\n"
    "; * Symbols and Macros\n"
    ";\n"
    "; Data Write Ports:\n"
    "DATA_LO:    EQU         $0000         ; Data High (PORTA)\n"
    "DATA_HI:    EQU         $0001         ; Data Low  (PORTB)\n"
    "DDRA        EQU         $0002\n"
    "DDRB        EQU         $0003\n"
    ";\n"
    "; Address Write Port: \n"
    "ADDERHI:    EQU         $0258         ; port P high order bits\n"
    "ADDERLO:    EQU         $0250         ; port M low order bits\n"
    "DDRM:       EQU         $0252        \n"
    "DDRP:       EQU         $025A\n"
    ";\n"
    "; Write Write Port: \n"
    "WRT:        EQU         $0008         ; Wrt Port (PORTE)\n"
    "DDRE:       EQU         $0009 \n"
    "; Chip Data\n"
    "            ORG         $3000         \n"
    "; =======================================================\n"
    "; = START Gen ROM\n"
    "DataStart:\n"
    "            ;         7   6   5   4   3   X   X   X   Mode\n"
    "            ; MV #    0   0   1   0   0   0   0   0   Immediate\n"
    "            ; MV      0   0   0   0   0   0   0   0   RR\n"
    "            ; ADD     0   0   0   1   0   0   0   0   RR\n"
    "            ; SUB     0   0   0   1   1   0   0   0   RR\n"
    "            ; BC      1   0   0   0   0   0   0   0   RR\n"
    "            ; IN      0   1   0   0   0   0   0   0   RR\n"
    "            ; OUT     0   1   0   0   1   0   0   0   RR\n"
    "            ;\n"
    "            ; --------------------------------\n"
    "            ; INST <4:0> | SRC <7:0> | DEST <2:0>\n"
    "            ;      5            8        3         = 16-bits\n"
    "            ;============================================================\n"
};


static const char code_bottom[] =
{
    "DataEnd:\n"
    "; = END Gen ROM    \n"
    "; =======================================================\n"
    "     \n"
    "; Stack                \n"
    "            DS.W        $0020         ; stack space\n"
    "stackPtr:\n"
    "; *\n"
    "; ******************************************************************\n"
    "; * Program Section\n"
    "; *\n"
    "            ORG         $3900           ; program start address\n"
    " _start:\n"
    "            LDS         #stackPtr       ; init stack pointer\n"
    "                                        ; setup I/O directions\n"
    "            LDAA        #$FF            ; Set Ports A, B, E, and\n"
    "            STAA        DDRA            ; P to be output ports\n"
    "            STAA        DDRB            ;\n"
    "            STAA        DDRE            ;\n"
    "            STAA        DDRM            ;\n"
    "            STAA        DDRP            ;\n"
    "            LDX         #$0000\n"
    "            JSR         sendData        ; set data lines to 0\n"
    "            NOP\n"
    "            JSR         setWriteOff     ; disable write\n"
    "                                        ; load data onto memory\n"
    "            LDY         #DataStart      ; start of data\n"
    "nextBlock:\n"
    "            JSR         setWriteOff\n"
    "            LDAA        1, Y+           ; A <- Address\n"
    "            JSR         sendAddress     ; write address\n"
    "            LDX         2, Y+           ; X <- Data \n"
    "            JSR         sendData        ; set data lines\n"
    "            JSR         setWriteOn\n"
    "            NOP\n"
    "            NOP\n"
    "            NOP\n"
    "            NOP\n"
    "            NOP\n"
    "            NOP\n"
    "            NOP\n"
    "            NOP\n"
    "            NOP\n"
    "            NOP\n"
    "            NOP\n"
    "            NOP\n"
    "            NOP\n"
    "            JSR         shortDelay\n"
    "            JSR         setWriteOff\n"
    "            JSR         shortDelay\n"
    "            CPY         #DataEnd      ; Test if at end of data  \n"
    "            BNE         nextBlock\n"
    "            SWI\n"
    "shortDelay:\n"
    "            PSHX\n"
    "            PSHY\n"
    "            LDX             #$FFFF\n"
    "loop:\n"
    "            LDY             #$0010\n"
    "loop2:      DEY\n"
    "            BNE             loop2\n"
    "            DEX\n"
    "            BNE             loop\n"
    "            PULY\n"
    "            PULX\n"
    "            RTS\n"
    "; *******************************************************\n"
    "; *\n"
    "; * sendAddress:\n"
    "; *\n"
    "; * Sends the content of register A across port M for the\n"
    "; * low order 6-bits and the high 2-bits on port p \n"
    "; *\n"
    "; * Ports: \n"
    "; *        [P1-1P0][M05-M00] (8-bits)\n"
    "; *\n"
    "; *******************************************************\n"
    "sendAddress:\n"
    "          STAA          ADDERLO\n"
    "          LSRA\n"
    "          LSRA\n"
    "          LSRA\n"
    "          LSRA\n"
    "          LSRA\n"
    "          LSRA\n"
    "          STAA          ADDERHI\n"
    "          RTS\n"
    "; *******************************************************\n"
    "; *\n"
    "; * sendData:\n"
    "; *\n"
    "; *  Sends the content of register X across the\n"
    "; *  8-bit DATA_LO and 8-bit DATA_HI\n"
    "; *\n"
    "; * Ports:\n"
    "; *        [B07-B00][A07-A00] (16-bits)\n"
    "; *\n"
    "; *******************************************************\n"
    "sendData:\n"
    "            XGDX\n"
    "            STAA        DATA_LO     ; store content\n"
    "            STAB        DATA_HI\n"
    "            XGDX\n"
    "            RTS            \n"
    "; *******************************************************\n"
    "; *\n"
    "; * setWriteOn:\n"
    "; *            \n"
    "; *  Turns the write bit on\n"
    "; *\n"
    "; *  Uses Register B, not preserved\n"
    "; *\n"
    "; *******************************************************\n"
    "setWriteOn:\n"
    "            LDAB        #$FF\n"
    "            STAB        WRT    \n"
    "            RTS             \n"
    "; *******************************************************\n"
    "; *\n"
    "; * setWriteOff: \n"
    "; *\n"
    "; *   Turns the write bit off  \n"
    "; *\n"
    "; *  Uses Register B, not preserved\n"
    "; * \n"
    "; *******************************************************        \n"
    "setWriteOff:  \n"
    "            LDAB        #$00\n"
    "            STAB        WRT   \n"
    "            RTS        \n"
    "; *******************************************************      \n"
    "; END\n"
    "            END\n"
    "; EOF\n"
};

void fatal(const char *format, ...);
void genFile(const char *fname);

int main(int argc, char *argv[])
{
    int i;
    
    if (argc < 2)
        fatal("no input files\n");
    
    for (i = 1; argv[i]; ++i)
        genFile(argv[i]);
    
    return EXIT_SUCCESS;
}


void genFile(const char *fname)
{
    int address;
    int data1, data2;
    char *s;
    FILE *f, *o;
    
    if (!(f = fopen(fname, "rb")))
        fatal("opening asm file: %s\n", strerror(errno));
    
    if (!(s = malloc(strlen(fname) + 7)))
        fatal("allocating mem: %s\n", strerror(errno));
    
    sprintf(s, "%s_gen.s", fname);
    
    if (!(o = fopen(s, "w+b")))
        fatal("creating output program: %s\n", strerror(errno));
    
    fputs(code_top, o);
    
    address = 0;
    while ((data1 = fgetc(f)) != EOF && (data2 = fgetc(f)) != EOF)
    {
        fprintf(o, "            DC.B        %d\n", address++);
        fprintf(o, "            DC.W        $%.2X%.2X\n", data1, data2);
    }
    
    fputs(code_bottom, o);
    
    free(s);
    fclose(o);
    fclose(f);
}

void fatal(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    fprintf(stderr, "** Simple Loader Error: ");
    vfprintf(stderr, format, args);
    va_end(args);
    exit(EXIT_FAILURE);
}
