; ===============================================
; =                                             =
; =  Chip:  V27K-CT                             =
; =  Test:  Pulse-width modulation App          =
; =    By:    David Schor                       =
; =                                             =
; = This program reads from input and generates =
; = the according controlled power via PWM.     =
; =                                             =
; = Input: low 3-bits of input                  =
; = Output: 8-bits of output                    =
; =                                             =
; = % Level - The on level is determined by the =
; =           3-bit numeric input given.        =
; =                                             =
; = Todo:                                       =
; =         Currently the % level is slightly   =
; =         off due to the fact that the jump   =
; =         happens during the off state. It    =
; =         could be improved by moving the on  =
; =         state above the input and calcula-  =
; =         tion point. Slightly evening out    =
; =         the on:off ratio.                   =
; ===============================================
;
; Constants:
period: equ     8
on:     equ     0xFF
off:    equ     0x00
;
; Program:
start:
        ; get input
        in      1               ; reg[1] = input
        
        ; calculate `input
        mv      #period, 0      ; reg[0] = period
        sub     1, 0            ; reg[0] = `input = period - input
        
        mv      #on, 5
        out     5               ; turn on input
        
        ; 255 used for both loops
        mv      #0xFF, 2
        
        ;
        ; Loop input times
        ;
l_on:   nop
        nop
        nop
        nop
        nop
        nop
        nop
        add     2, 1            ; reg[1] = 255 + reg[1]
        bc      l_on            ; iterate input number of times
        
        mv      #off,5
        out     5               ; turn off input
        ; Loop `input times
        
        
        ;
        ; Loop `input amount of times
        ;
l_off:  nop
        nop
        nop
        nop
        nop
        nop
        nop
        add     2, 0            ; reg[0] = 255 + reg[0]
        bc      l_off           ; iterate `input number of times
        
        
        
        ; Go back
        mv      #5,5
        mv      #6,6
        sub     6,5             ; unconditional jmp
        bc      start
    
        nop