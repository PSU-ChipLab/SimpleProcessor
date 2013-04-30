; ===============================================
; =                                             =
; =  Chip:  Simple, V0                          =
; =  Test:  Branch Test                         =
; =  By:    David Schor                         =
; =                                             =
; = This program generates an alternating       =
; = sequence of bits on its output. If the      =
; = branch operation failed, the output will    =
; = also show all bits on at the same time.     =
; =                                             =
; ===============================================
;
loop:   mv      #0b10101010, 0
        out     0
        nop                     ; delay for visual inspection
        nop
        mv      #0b01010101, 0
        out     0
        mv      #5,5
        mv      #6,6
        sub     6,5             ; unconditional jmp
        bc      loop
        
        ; make sure we don't get here
        mv #0xFF, 0
        out 0 
