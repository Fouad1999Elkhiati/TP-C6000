;******************************************************************************
;* FILENAME                                                                   *
;*   IIR_filters_second_order_asm.asm                                         *
;*                                                                            *
;* DESCRIPTION                                                                *
;*   Assembly language implementation of second order IIR filter for the      *
;*   TMS320C6713 DSK (C6000). Port of the TMS320C5416 version by              *
;*   Richard Sikora.                                                          *
;*                                                                            *
;*   Same computation as the C5416 code, bit for bit:                         *
;*                                                                            *
;*   C5416 (FRCT=1, OVM=1, SXM=1)    C6000 equivalent                         *
;*   MPY  *AR3+,A                    SMPY  (2*a*b)                            *
;*   MAS  *AR3,*AR4,A                SMPY + SSUB (32-bit saturated subtract)  *
;*   MAC  *AR3,*AR4,A                SMPY + SADD (32-bit saturated add)       *
;*   SFTA A,#-7,A                    SHR   A8,7,A8                            *
;*   STH  A,*(_delay)                SHR   A8,16 / STH                        *
;*   DELAY                           STH of the previous value                *
;*   return AL after SFTA A,#-9      SHR   A8,9 then EXT (low 16 bits)        *
;*                                                                            *
;*   Only C62x instructions are used (C6713, C6416, C6455, C674x).            *
;*   Delay slots: LDH 4, SMPY 1, B 5.                                         *
;*                                                                            *
;*  C calling convention: A4 = &coefficients[0], B4 = input, result in A4,    *
;*  return address in B3. Only A0-A9 / B0-B9 are used.                        *
;*                                                                            *
;*  Revision: 1.01 (C5416)                                                    *
;*   3rd December 2002. Shift increased from 3 to 7 to avoid overload on      *
;*   bandpass filters.                                                        *
;*  Revision: 1.00 (C5416)                                                    *
;*   21st November 2002. Created by Richard Sikora from                       *
;*   iir_filters_second_order.c                                               *
;*                                                                            *
;******************************************************************************

; Numerator coefficients (index of 16-bit elements)
; (On the C54 they were called B0, B1, B2, A0, A1, A2. On the C6000 these
;  names are register names, so they are called NUM_B0 ... DEN_A2 here.)
NUM_B0  .set 0
NUM_B1  .set 1
NUM_B2  .set 2

; Denominator coefficients
DEN_A0  .set 3
DEN_A1  .set 4
DEN_A2  .set 5

        .bss    _delay, 6, 4            ; delay[0], delay[1], delay[2] (16 bits)

; C function names: with the COFF ABI (CCS 3.3, dsk6713bsl.lib) they start
; with an underscore, with the EABI / ELF (default of recent CCS versions)
; they do not. These lines make this file work with both.
        .if $defined(__TI_EABI__)
        .if __TI_EABI__
        .asg    second_order_IIR_asm_initialise, _second_order_IIR_asm_initialise
        .asg    second_order_IIR_direct_form_II_asm, _second_order_IIR_direct_form_II_asm
        .endif
        .endif

        .global _second_order_IIR_asm_initialise
        .global _second_order_IIR_direct_form_II_asm

        .text

;***************************************************************
;* FUNCTION DEF: _second_order_IIR_asm_initialise              *
;***************************************************************

_second_order_IIR_asm_initialise:

        MVKL    _delay, A5
        MVKH    _delay, A5
        ZERO    A2
        STH     A2, *A5                 ; delay[0] = 0
        STH     A2, *+A5[1]             ; delay[1] = 0
        STH     A2, *+A5[2]             ; delay[2] = 0

        B       B3                      ; Return
        NOP     5


;***************************************************************
;* FUNCTION DEF: _second_order_IIR_direct_form_II_asm          *
;***************************************************************

; The value &coefficients[0] is passed to this function in A4,
; the input in B4. Register A8 holds the variable temp.
; Order of coefficients is B0, B1/2, B2, A0, A1/2, A2

_second_order_IIR_direct_form_II_asm:

        MVKL    _delay, A5
        MVKH    _delay, A5              ; A5 points to delay[0]
        MV      B4, A8                  ; A8 = input

        LDH     *+A4[DEN_A0], A2        ; A2 = coefficients[A0]
        LDH     *+A4[DEN_A1], A3        ; A3 = coefficients[A1]  (A1/2)
        LDH     *+A4[DEN_A2], A6        ; A6 = coefficients[A2]
        LDH     *+A5[1], A7             ; A7 = delay[1]
        LDH     *+A5[2], A9             ; A9 = delay[2]
        NOP     4

;  temp = (((long)coefficients[A0] * input ) >> 7 ); /* Divide by 128 */

        SMPY    A2, A8, A8              ; temp = input * coefficients[A0]
        NOP     1
        SHR     A8, 7, A8               ; temp >>= 7

        SMPY    A3, A7, A0              ; A0 = coefficients[A1] * delay[1]
        NOP     1
        SSUB    A8, A0, A8              ; temp -= coefficients[A1] * delay[1]
        SSUB    A8, A0, A8              ; temp -= coefficients[A1] * delay[1]

        SMPY    A6, A9, A0              ; A0 = coefficients[A2] * delay[2]
        NOP     1
        SSUB    A8, A0, A8              ; temp -= coefficients[A2] * delay[2]

;  temp >>= 15;  /* temp /= coefficients[A0] */
;  delay[0] = (signed int) temp;

        SHR     A8, 16, A1              ; A1 = new delay[0] (C54: STH A)
        STH     A1, *A5                 ; delay[0] = A >> 16

        LDH     *+A4[NUM_B0], A2        ; A2 = coefficients[B0]
        LDH     *+A4[NUM_B1], A3        ; A3 = coefficients[B1]  (B1/2)
        LDH     *+A4[NUM_B2], A6        ; A6 = coefficients[B2]
        NOP     4

        SMPY    A6, A9, A8              ; temp = coefficients[B2] * delay[2]
        NOP     1

        SMPY    A3, A7, A0              ; A0 = coefficients[B1] * delay[1]
        NOP     1
        SADD    A8, A0, A8              ; temp += coefficients[B1] * delay[1]
        SADD    A8, A0, A8              ; temp += coefficients[B1] * delay[1]

        SMPY    A2, A1, A0              ; A0 = coefficients[B0] * delay[0]
        NOP     1
        SADD    A8, A0, A8              ; temp += coefficients[B0] * delay[0]

        STH     A7, *+A5[2]             ; delay[2] = delay[1]
        STH     A1, *+A5[1]             ; delay[1] = delay[0]

        SHR     A8, 9, A8               ; temp /= coefficients[A0] * 128 (C54: SFTA A,#(-16+7))
        EXT     A8, 16, 16, A4          ; Return the low 16 bits (C54: AL)

        B       B3                      ; Return
        NOP     5


;******************************************************************************
;* End of iir_filters_second_order_asm.asm                                    *
;******************************************************************************
