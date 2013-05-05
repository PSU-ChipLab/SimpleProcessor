; ===============================================
; =                                             =
; =  Chip:  Simple, V0                          =
; =  Test:  Input/Output Test                   =
; =  By:    David Schor                         =
; =                                             =
; = This program reads from input port and      =
; = prints the very same value onto the output  =
; = port.                                       =
; =                                             =
; ===============================================
;
loop:   nop
        nop
        in      0       ; read into reg 0
        in      1       ; read into reg 1
        nop
        nop
        nop
        out     0       ; print reg 0
        out     1       ; print reg 1
        nop
        nop
        nop
        mv      #2,2
        mv      #3,3
        sub     3,2
        bc      loop    ; loop back
        
        ; make sure we don't get here
        mv      #0xFF, 0
        out     0 
