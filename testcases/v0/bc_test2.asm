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
; =   Test Type:    Branch Test                                                     =
; =          By:    David Schor                                                     =
; =         For:    Penn State Chip Lab                                             =
; =                                                                                 =
; = This program turns on the four most significant bits and alternates the lower   =
; = four bits. This will allow for a visual comparison - the lower four bits should =
; = be dimmed at 50%, given appropriately high clock frequency.                     =
; =                                                                                 =
; ===================================================================================
;
loop:       mv          #0b11111111, 0
            out         0
            nop                             ; delay for visual inspection
            nop
            mv          #0b11110000, 0
            out         0
            mv          #5,5
            mv          #6,6
            sub         6,5                 ; unconditional jmp
            bc          loop
            
            ; make sure we don't get here
            mv          #0xFF, 0
            out         0 
