PAS - The Penn State Assembler
========

`PAS`, the Penn State Assembler, is a program capable of converting the Simple Chip instruction set into an object code which can then be executed by the processor.
PAS is a two-stage assembler with constants, lables, and comments support. PAS is designed to extensible as to support future versions of the Simple Chip.

Build Steps
------

`PAS` is coded in 100% [ANSI C (C89)](http://en.wikipedia.org/wiki/ANSI_C) and should compile and run out of the box on all platforms.
On Unix and GNU/Linux systems, a [make file](http://en.wikipedia.org/wiki/Makefile) is provided.

```
   make
   make clean
```

Simple, V0 ISA
------

The full specification datasheet for the `Simple Chip, V0` can be [found here](/specs/v0/ChipV0.pdf).

The currently supported instructions are `NOP`, `MV`, `ADD`, `SUB`, `BC`, `IN`, and `OUT`.

```
   ; Sample Code (does nothing particularly useful)
   A:
      ADD     1, 2
      NOP
      SUB     2, 9
      NOP
      BC      A
      MV      9, 1
      MV      #0xF, 1
```

Additionally, a number of pseudo instructions are supported `INC`, `DEC`, `DECZ`, `CLR`, `BLT`, `BGT`, and `BNZ`.
It's important to understand that the assembler may use `register 0` when necessary when expanding pseudo instructions.

```
   ; Sample Code (does nothing particularly useful)
   A:
      BLT     1, 2, 3
      CLR     5
      CLR     2
      DECZ    2
   B: BC      A
      MV      9, 1
      MV      #0xF, 1
```

The assembler also supports the equate assembler directive for constants.

```
foo:  equ   0xFF
bar:  equ   0x00
baz:  equ   0x12

   A:
      BLT     1, 2, 3
      MV      #foo, 2
      DECZ    2
      MV      #bar, 4
   B: BC      A
      ADD     #baz, 4
      MV      #0xF, 1
```


A set of sample programs can be [found here](/testcases/v0).
