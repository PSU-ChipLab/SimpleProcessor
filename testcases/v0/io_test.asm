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
; =   Test Type:    Input/Output Ports Test                                         =
; =          By:    David Schor                                                     =
; =         For:    Penn State Chip Lab                                             =
; =                                                                                 =
; = This program reads from input port and prints the very same value onto the      =
; = output port.                                                                    =
; =                                                                                 =
; ===================================================================================
;
loop:       nop
            nop
            in          0       ; read into reg 0
            in          1       ; read into reg 1
            nop
            nop
            nop
            out         0       ; print reg 0
            out         1       ; print reg 1
            nop
            nop
            nop
            mv          #2,2
            mv          #3,3
            sub         3,2
            bc          loop    ; loop back
            
            ; make sure we don't get here
            mv          #0xFF, 0
            out         0 
