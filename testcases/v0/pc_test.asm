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
; =   Test Type:    Program Counter Wrap-Around Test                                =
; =          By:    David Schor                                                     =
; =         For:    Penn State Chip Lab                                             =
; =                                                                                 =
; = This program tests the PC by simply executing NOPs for most of the instruction  =
; = memory. The first instruction turns all bits on the output port and turns them  =
; = back off at the middle of the instruction memory space.                         =
; =                                                                                 =
; ===================================================================================
;
            mv          #0xFF, 0
            out         0
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
            mv          #0x00, 0
            out         0
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