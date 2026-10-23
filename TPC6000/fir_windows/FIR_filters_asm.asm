;******************************************************************************
;* FILENAME                                                                   *
;*   FIR_filters_asm.asm                                                      *
;*                                                                            *
;* DESCRIPTION                                                                *
;*   Finite Impulse Response Filters for the TMS320C6713 DSK (C6000).         *
;*   Port of the TMS320C5416 version by Richard Sikora.                       *
;*                                                                            *
;*   The computation is the same as on the C5416, bit for bit:                *
;*                                                                            *
;*   C5416 (FRCT=1, OVM=1)          C6000 equivalent                          *
;*   ---------------------          ----------------                          *
;*   MAC  *AR3+,*AR4+,A             SMPY h,x,p   (p = 2*h*x, "FRCT" shift)    *
;*                                  SADD acc,p,acc (32-bit saturation, "OVM") *
;*   MACR (rounded last MAC)        same + SADD acc,0x8000,acc                *
;*   SFTA A,-16,A                   SHR  acc,16,A4                            *
;*   RPT #(N-2) / DELAY *AR4-       loop: LDH buffer[k] / STH buffer[k+1]     *
;*   accumulator A (40 bits)        register A8 (32 bits, saturated as A is   *
;*                                  saturated to 32 bits by OVM)              *
;*                                                                            *
;*   Only C62x instructions are used: the code runs on C6713 (C67x), C6416    *
;*   (C64x), C6455 (C64x+) and C674x.                                         *
;*                                                                            *
;*   The code is deliberately NOT software-pipelined: one instruction per     *
;*   line, with the delay slots filled by NOPs, so that it reads like the     *
;*   C54 version. C6000 delay slots used here:                                *
;*      LDH  : 4 delay slots  (data usable 5 cycles after the load)           *
;*      SMPY : 1 delay slot                                                   *
;*      B    : 5 delay slots  (the 5 instructions after B are executed)       *
;*                                                                            *
;*  C calling convention (TI C6000 COFF ABI, as used with dsk6713bsl.lib):    *
;*      1st argument in A4, 2nd in B4, 3rd in A6, result returned in A4,      *
;*      return address in B3. A0-A9 and B0-B9 can be used freely; A10-A15,    *
;*      B10-B15 (B15 = stack pointer) are not touched by these functions.     *
;*                                                                            *
;*  The number of FIR filter elements is controlled by N                      *
;*                                                                            *
;******************************************************************************

N       .set    51

; Four buffers of N = 51 elements (16 bits each = 2*N bytes), word aligned
; buffer[0] holds the newest sample x(n), buffer[k] holds x(n-k).

        .bss    _buffer1, 2*N, 4
        .bss    _buffer2, 2*N, 4
        .bss    _buffer3, 2*N, 4
        .bss    _buffer4, 2*N, 4

; C function names: with the COFF ABI (CCS 3.3, dsk6713bsl.lib) they start
; with an underscore, with the EABI / ELF (default of recent CCS versions)
; they do not. These lines make this file work with both.
        .if $defined(__TI_EABI__)
        .if __TI_EABI__
        .asg    FIR_filters_asm_initialize, _FIR_filters_asm_initialize
        .asg    FIR_filter_asm, _FIR_filter_asm
        .asg    FIR_filter_asm_2, _FIR_filter_asm_2
        .asg    FIR_dual_filter_asm, _FIR_dual_filter_asm
        .asg    FIR_dual_filter_variable_asm, _FIR_dual_filter_variable_asm
        .endif
        .endif

; Give functions global scope to make them available to other functions

        .global _FIR_filters_asm_initialize
        .global _FIR_filter_asm
        .global _FIR_filter_asm_2
        .global _FIR_dual_filter_asm
        .global _FIR_dual_filter_variable_asm

        .text

;******************************************************************************
;* FUNCTION DEF: _FIR_filters_asm_initialize                                  *
;******************************************************************************
;*                                                                            *
;* Fill four buffers used for FIR filters with zeroes to prevent noise and    *
;* clicks.                                                                    *
;*                                                                            *
;******************************************************************************

_FIR_filters_asm_initialize:

        ZERO    A2                      ; A2 = 0

        MVKL    _buffer1, A5            ; A5 points to buffer1[0]
        MVKH    _buffer1, A5
        MVK     N, B0                   ; N elements
clr1:   STH     A2, *A5++               ; Fill each element with zero
        SUB     B0, 1, B0
 [B0]   B       clr1
        NOP     5

        MVKL    _buffer2, A5            ; A5 points to buffer2[0]
        MVKH    _buffer2, A5
        MVK     N, B0
clr2:   STH     A2, *A5++
        SUB     B0, 1, B0
 [B0]   B       clr2
        NOP     5

        MVKL    _buffer3, A5            ; A5 points to buffer3[0]
        MVKH    _buffer3, A5
        MVK     N, B0
clr3:   STH     A2, *A5++
        SUB     B0, 1, B0
 [B0]   B       clr3
        NOP     5

        MVKL    _buffer4, A5            ; A5 points to buffer4[0]
        MVKH    _buffer4, A5
        MVK     N, B0
clr4:   STH     A2, *A5++
        SUB     B0, 1, B0
 [B0]   B       clr4
        NOP     5

        B       B3                      ; Return
        NOP     5


;******************************************************************************
;* FUNCTION DEF: _FIR_filter_asm                                              *
;******************************************************************************
;*                                                                            *
;* FIR filter for first channel. Can be used for high pass, low pass          *
;* or other FIR filters.                                                      *
;*                                                                            *
;* A4 = &filter_constants[0]   B4 = input   Returns y(n) in A4                *
;*                                                                            *
;******************************************************************************

_FIR_filter_asm:

        ; Start by shuffling values in buffer along and inserting new value
        ; at buffer[0]   (C54: RPT #(N-2) / DELAY *AR4-)
        ; for k = N-2 down to 0 : buffer[k+1] = buffer[k]

        MVKL    _buffer1, A5
        MVKH    _buffer1, A5
        ADDK    2*(N-2), A5             ; A5 points to buffer1[N-2] = buffer1[49]
        MVK     N-1, B0                 ; N-1 moves
sh1:    LDH     *A5, A2                 ; A2 = buffer[k]
        NOP     4
        STH     A2, *+A5[1]             ; buffer[k+1] = buffer[k]
        SUB     A5, 2, A5               ; k = k - 1
        SUB     B0, 1, B0
 [B0]   B       sh1
        NOP     5

        MV      B4, A3                  ; A3 = input
        STH     A3, *+A5[1]             ; New input to beginning of buffer:
                                        ; A5 now points to buffer[-1], so
                                        ; *+A5[1] is buffer1[0]
        ADD     A5, 2, A5               ; A5 points to buffer1[0]

        ; Multiplications with accumulation   (C54: RPTZ A,#(N-2) / MAC)

        ZERO    A8                      ; Clear accumulator
        MVK     N-1, B0                 ; N-1 multiply-accumulates
mac1:   LDH     *A4++, A2               ; A2 = h[i]
        LDH     *A5++, A7               ; A7 = x(n-i)
        NOP     4
        SMPY    A2, A7, A3              ; A3 = 2*h[i]*x(n-i)
        NOP     1
        SADD    A8, A3, A8              ; acc = acc + A3, saturated
        SUB     B0, 1, B0
 [B0]   B       mac1
        NOP     5

        ; Last one with rounding           (C54: MACR *AR3+,*AR4+,A)

        LDH     *A4, A2                 ; h[N-1]
        LDH     *A5, A7                 ; x(n-N+1)
        NOP     4
        SMPY    A2, A7, A3
        NOP     1
        SADD    A8, A3, A8
        MVK     1, A9
        SHL     A9, 15, A9              ; A9 = 0x8000 = rounding constant
        SADD    A8, A9, A8              ; Round up the last one

        SHR     A8, 16, A4              ; Remove fractional part (C54: SFTA A,-16)
                                        ; Ready to return output in A4
        B       B3                      ; Return
        NOP     5


;******************************************************************************
;* FUNCTION DEF: _FIR_filter_asm_2                                            *
;******************************************************************************
;*                                                                            *
;* FIR filter for second channel. Can be used for high pass, low pass         *
;* or other FIR filters. Same as _FIR_filter_asm, with buffer2.               *
;*                                                                            *
;******************************************************************************

_FIR_filter_asm_2:

        MVKL    _buffer2, A5
        MVKH    _buffer2, A5
        ADDK    2*(N-2), A5             ; A5 points to buffer2[N-2]
        MVK     N-1, B0
sh2:    LDH     *A5, A2                 ; buffer[k+1] = buffer[k]
        NOP     4
        STH     A2, *+A5[1]
        SUB     A5, 2, A5
        SUB     B0, 1, B0
 [B0]   B       sh2
        NOP     5

        MV      B4, A3                  ; A3 = input
        STH     A3, *+A5[1]             ; New input to buffer2[0]
        ADD     A5, 2, A5               ; A5 points to buffer2[0]

        ZERO    A8                      ; Clear accumulator
        MVK     N-1, B0
mac2:   LDH     *A4++, A2
        LDH     *A5++, A7
        NOP     4
        SMPY    A2, A7, A3
        NOP     1
        SADD    A8, A3, A8
        SUB     B0, 1, B0
 [B0]   B       mac2
        NOP     5

        LDH     *A4, A2                 ; Last one with rounding
        LDH     *A5, A7
        NOP     4
        SMPY    A2, A7, A3
        NOP     1
        SADD    A8, A3, A8
        MVK     1, A9
        SHL     A9, 15, A9              ; 0x8000
        SADD    A8, A9, A8

        SHR     A8, 16, A4              ; Remove fractional part
        B       B3
        NOP     5


;******************************************************************************
;* FUNCTION DEF: _FIR_dual_filter_asm                                         *
;******************************************************************************
;*                                                                            *
;* FIR filter for producing both high pass and low pass filter from a single  *
;* set of coefficients.                                                       *
;*                                                                            *
;* Returns a 32-bit int in A4:                                                *
;*   high 16 bits = filter output        (C54: AH)                            *
;*   low  16 bits = mid point - output   (C54: BL, ORed into AL)              *
;*                                                                            *
;******************************************************************************

_FIR_dual_filter_asm:

        ; Shuffle values in buffer3 along and insert new value at buffer3[0]

        MVKL    _buffer3, A5
        MVKH    _buffer3, A5
        ADDK    2*(N-2), A5             ; A5 points to buffer3[N-2]
        MVK     N-1, B0
sh3:    LDH     *A5, A2
        NOP     4
        STH     A2, *+A5[1]
        SUB     A5, 2, A5
        SUB     B0, 1, B0
 [B0]   B       sh3
        NOP     5

        MV      B4, A3                  ; A3 = input
        STH     A3, *+A5[1]             ; New input to buffer3[0]
        ADD     A5, 2, A5               ; A5 points to buffer3[0]
        MV      A5, A6                  ; Keep &buffer3[0] in A6

        ; Series of multiplications with accumulation. Last with rounding.

        ZERO    A8
        MVK     N-1, B0
mac3:   LDH     *A4++, A2
        LDH     *A5++, A7
        NOP     4
        SMPY    A2, A7, A3
        NOP     1
        SADD    A8, A3, A8
        SUB     B0, 1, B0
 [B0]   B       mac3
        NOP     5

        LDH     *A4, A2
        LDH     *A5, A7
        NOP     4
        SMPY    A2, A7, A3
        NOP     1
        SADD    A8, A3, A8
        MVK     1, A9
        SHL     A9, 15, A9              ; 0x8000
        SADD    A8, A9, A8              ; Round up the last one

        SHR     A8, 16, A8              ; product = filter output (C54: SFTA A,-16,B)

        LDH     *+A6[(N-1)/2], A2       ; Mid point i.e. buffer3[25]
        NOP     4
        SUB     A2, A8, A2              ; Mid point + (- product)  (C54: NEG B / ADD)
        EXTU    A2, 16, 16, A2          ; Keep the low 16 bits     (C54: AND #0FFFFh,B)

        SHL     A8, 16, A8              ; product in the high 16 bits (C54: AH)
        OR      A8, A2, A4              ; Return high OR low in A4 (C54: OR B,0,A)

        B       B3
        NOP     5


;*******************************************************************************
;* FUNCTION DEF: _FIR_dual_filter_variable_asm                                 *
;*******************************************************************************
;*                                                                             *
;* When function is called from C code:                                        *
;* A4 contains address of coefficients                                         *
;* B4 contains latest input to filter                                          *
;* A6 contains length of filter (limited to 3 ... N)                           *
;*                                                                             *
;* RETURNS: filter output in the high 16 bits, mid point - output in the low   *
;*          16 bits of A4 (as _FIR_dual_filter_asm)                            *
;*******************************************************************************

_FIR_dual_filter_variable_asm:

        ; Test for length less than 3. If so set to 3.
        MVK     3, A2
        CMPLT   A6, A2, A1              ; A1 = (length < 3)
 [A1]   MVK     3, A6

        ; Test for length greater than N. If so set to N.
        MVK     N, A2
        CMPGT   A6, A2, A1              ; A1 = (length > N)
 [A1]   MVK     N, A6

        ; Shuffle values in buffer4 one place along

        MVKL    _buffer4, A5
        MVKH    _buffer4, A5
        ADDK    2*(N-2), A5             ; A5 points to buffer4[N-2]
        MVK     N-1, B0
sh4:    LDH     *A5, A2
        NOP     4
        STH     A2, *+A5[1]
        SUB     A5, 2, A5
        SUB     B0, 1, B0
 [B0]   B       sh4
        NOP     5

        MV      B4, A3                  ; A3 = input
        STH     A3, *+A5[1]             ; Save new input in buffer4[0]
        ADD     A5, 2, A5               ; A5 points to buffer4[0]
        MV      A5, A0                  ; Keep &buffer4[0] in A0

        ; Calculate ( N - length ) / 2 and offset both pointers by it

        MVK     N, A2
        SUB     A2, A6, A2
        SHR     A2, 1, A2               ; A2 = ( N - length ) / 2
        ADD     A2, A2, A2              ; in bytes (16-bit elements)
        ADD     A5, A2, A5              ; A5 points to buffer4[(N-length)/2]
        ADD     A4, A2, A4              ; A4 points to coefficients[(N-length)/2]

        ZERO    A8                      ; Clear accumulator
        MV      A6, B0
        SUB     B0, 1, B0               ; length - 1 multiply-accumulates
macv:   LDH     *A4++, A2
        LDH     *A5++, A7
        NOP     4
        SMPY    A2, A7, A3
        NOP     1
        SADD    A8, A3, A8
        SUB     B0, 1, B0
 [B0]   B       macv
        NOP     5

        LDH     *A4, A2                 ; Round up the last one
        LDH     *A5, A7
        NOP     4
        SMPY    A2, A7, A3
        NOP     1
        SADD    A8, A3, A8
        MVK     1, A9
        SHL     A9, 15, A9              ; 0x8000
        SADD    A8, A9, A8

        SHR     A8, 16, A8              ; product

        LDH     *+A0[(N-1)/2], A2       ; Mid point i.e. buffer4[(N-1)/2]
        NOP     4
        SUB     A2, A8, A2              ; Mid point + (- product)
        EXTU    A2, 16, 16, A2          ; Low 16 bits

        SHL     A8, 16, A8
        OR      A8, A2, A4              ; Return high OR low in A4

        B       B3
        NOP     5

;******************************************************************************
;* End of FIR_filters_asm.asm                                                 *
;******************************************************************************
