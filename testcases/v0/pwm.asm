; ===================================================================================
; =                                                                                 =
; = Copyright (C) 2013 David Schor                                                  =
; =                                                                                 =
; =                                                                                 =
; = Permission is hereby granted, free of charge, to any person obtaining a copy    =
; = of this software and associated documentation files (the "Software"), to deal   =
; = in the Software without restriction, including without limitation the rights    =
; = to use, copy, modify, merge, publish, distribute, sublicense, and/or sell       =
; = copies of the Software, and to permit persons to whom the Software is           =
; = furnished to do so, subject to the following conditions:                        =
; =                                                                                 =
; = The above copyright notice and this permission notice shall be included in      =
; = all copies or substantial portions of the Software.                             =
; =                                                                                 =
; = THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      =
; = IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        =
; = FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     =
; = AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          =
; = LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   =
; = OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN       =
; = THE SOFTWARE.                                                                   =
; =                                                                                 =
; ===================================================================================
; =                                                                                 =
; =        Chip:    Simple, V0                                                      =
; =   Test Type:    Pulse-width modulation Program                                  =
; =          By:    David Schor                                                     =
; =         For:    Penn State Chip Lab                                             =
; =                                                                                 =
; = Description:    This program reads from the input port a 3-bit value and        =
; =                 generates the according controlled power via PWM.               =
; =                                                                                 =
; =       Input:    Low 3-bits of the input port.                                   =
; =      Output:    8-bit of the output port.                                       =
; =                                                                                 =
; =     % Level:    The ON level is determined by the 3-bit numeric input value.    =
; =                                                                                 =
; =        Todo:    Current the percent level is slightly off due to the fact that  =
; =                 the jump occurs during the off state. It could be improved by   =
; =                 moving the on state above the input and calculating point.      =
; =                 Slightly evening out the on:off ratio.                          =
; =                                                                                 =
; ===================================================================================
;
; Constants:
period:     equ         8
on:         equ         0xFF
off:        equ         0x00
;
; Program:
start:
            ; get input
            in          1               ; reg[1] = input
        
            ; calculate `input
            mv          #period, 0      ; reg[0] = period
            sub         1, 0            ; reg[0] = `input = period - input
        
            mv          #on, 5
            out         5               ; turn on input
        
            ; 255 used for both loops
            mv          #0xFF, 2
        
        ;
        ; Loop input times
        ;
l_on:       nop
            nop
            nop
            nop
            nop
            nop
            nop
            add         2, 1            ; reg[1] = 255 + reg[1]
            bc          l_on            ; iterate input number of times
        
            mv          #off,5
            out         5               ; turn off input
            ; Loop `input times
        
        
            ;
            ; Loop `input amount of times
            ;
l_off:      nop
            nop
            nop
            nop
            nop
            nop
            nop
            add         2, 0            ; reg[0] = 255 + reg[0]
            bc          l_off           ; iterate `input number of times
        
            
            
            ; Go back
            mv          #5,5
            mv          #6,6
            sub         6,5             ; unconditional jmp
            bc          start
        
            nop