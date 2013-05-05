; ===============================================
; =                                             =
; =  Chip:  Simple, V0                          =
; =  Test:  Branch Test                         =
; =  By:    David Schor                         =
; =                                             =
; = This program turns on the upper four bits   =
; = and alternates the lower four bits. This    =
; = will allow to compare the first four bits   =
; = which should be dimmed at 50% under higher  =
; = frequency compared to the higher four bits. =
; ===============================================
;
loop:   mv      #0b11111111, 0
        out     0
        nop                     ; delay for visual inspection
        nop
        mv      #0b11110000, 0
        out     0
        mv      #5,5
        mv      #6,6
        sub     6,5             ; unconditional jmp
        bc      loop
        
        ; make sure we don't get here
        mv #0xFF, 0
        out 0 
