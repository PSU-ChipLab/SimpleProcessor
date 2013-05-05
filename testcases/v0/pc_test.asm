; ===============================================
; =                                             =
; =  Chip:  Simple, V0                          =
; =  Test:  Branch Test                         =
; =  By:    David Schor                         =
; =                                             =
; = This is a simple program to test the PC     =
; = and make sure it wraps around. In the proc- =
; = ess the output port is turned all high then =
; = all low.                                    =
; =                                             =
; ===============================================
; 
        mv      #0xFF, 0
        out     0
        nop                     ; delay for visual inspection
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        mv      #0x00, 0
        out     0
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop
        nop