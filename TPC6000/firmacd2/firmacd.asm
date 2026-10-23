;******************************************************************************
;* firmacd.asm  -  FIR filter, TMS320C6713 (C6000) version of the C5416        *
;*                 MACD example of the lab (TP DSP, part 1).                  *
;*                                                                            *
;* Same filter, same data, same results as the C5416 version:                 *
;*   y(n) = bits 31..16 of  sum_{i=0}^{N-1} 2 * adr_coef[i] * x(n-(N-1-i))    *
;*                                                                            *
;* What the C5416 did in ONE instruction, MACD *AR1-, adr_coef, A, is done    *
;* here by a small loop:                                                      *
;*   LDH  x = *AR1              (data)                                        *
;*   LDH  h = adr_coef[PAR++]   (coefficient)                                 *
;*   STH  x -> *(AR1+1)         (the "D"elay part of MACD)                    *
;*   MPY  x*h, SHL 1            (product, shifted left by 1 as FRCT=1)        *
;*   ADD  acc                   (OVM=0 on the C5416: no saturation)           *
;*   AR1 = AR1 - 1                                                            *
;*                                                                            *
;* Differences with the C5416 to know for the lab:                            *
;* - The C6000 has a single memory space (no program/data pages): the         *
;*   coefficients are simply in the ".coef" section of the internal RAM.      *
;* - There is no DP register, no FRCT/OVM mode bit: the shift by 1 of the     *
;*   product (FRCT) is a SHL instruction.                                     *
;* - Addresses are in bytes: one 16-bit sample = 2 bytes.                     *
;* - Delay slots: LDH 4, MPY 1, B 5 (the 5 instructions after B execute).     *
;* - "in" and "out" are 32-bit words (CCS File I/O on the C6000 transfers     *
;*   32-bit words). in: the 16-bit sample is read in the low half (LDH).      *
;*   out: y(n) sign-extended to 32 bits, so a graph of "out" in 16-bit or    *
;*   32-bit signed shows the same value.                                      *
;*                                                                            *
;* Registers (C5416 -> C6000):                                                *
;*   AR2 -> A2 (adr_debut_dat)   AR1 -> A1 (current data pointer)             *
;*   PAR -> A7 (coefficient pointer)  A (accumulator) -> A8                   *
;*   RC (repeat counter) -> B0                                                *
;******************************************************************************

        .def    _start
        .global adr_debut_dat, adr_fin_dat, adr_coef, _start
        .global in, out

        .sect   ".coef"
adr_coef  .short  0x400
          .short  0x400,0x400,0x400,0x400,0x400
          .short  0x400,0x400,0x400,0x400,0x400
          .short  0x400,0x400,0x400,0x400,0x400  ; 0x400 = 1/32 en mode fractionnaire

N       .set    16                      ; 16 --> N
        .bss    adr_debut_dat, 2*(N+1), 4  ; N+1 16-bit words at adr_debut_dat
adr_fin_dat .set adr_debut_dat+2*(N-1)  ; address of the last element (16 bits = 2 bytes)
        .bss    in, 4, 4                ; 1 word for in
        .bss    out, 4, 4               ; 1 word for out

        .text
* Initialisation (C54: LD #0,DP / SSBX FRCT -> nothing to set on the C6000)
_start:
        MVKL    adr_debut_dat, A2       ; (adr_debut_dat) --> A2  (C54: AR2)
        MVKH    adr_debut_dat, A2

* Init de la zone memoire a 0 (N words)
        ZERO    A3                      ; 0 --> A3
        MV      A2, A4
        MVK     N, B0                   ; N --> B0 (repeat counter)
init:   STH     A3, *A4++               ; 0 --> *A4, A4++
        SUB     B0, 1, B0
 [B0]   B       init
        NOP     5

* Boucle sans fin
debut:
* Initialisation de A1 (C54: AR1)       <-- PROBE POINT: infir.dat -> in
        MVKL    adr_fin_dat, A1         ; (adr_fin_dat) --> A1
        MVKH    adr_fin_dat, A1

* Lecture de x(n) a l'adresse "in" (remplie a partir du fichier infir.dat)
        MVKL    in, A5
        MVKH    in, A5
        LDH     *A5, A6                 ; *in --> A6
        NOP     4
        STH     A6, *A2                 ; A6 --> *A2  (x(n) -> adr_debut_dat[0])

* Boucle de filtrage (C54: RPTZ A,#N-1 / MACD *AR1-, adr_coef, A)
        ZERO    A8                      ; 0 --> acc
        MVKL    adr_coef, A7            ; PAR = adr_coef
        MVKH    adr_coef, A7
        MVK     N, B0                   ; N iterations
filt:   LDH     *A1, A3                 ; x = *A1
        LDH     *A7++, A9               ; h = adr_coef[PAR++]
        NOP     4
        STH     A3, *+A1[1]             ; *A1 --> *(A1+1)  (delay)
        MPY     A3, A9, A3              ; x*h (16 x 16 -> 32 bits)
        NOP     1
        SHL     A3, 1, A3               ; FRCT : product shifted left by 1
        ADD     A8, A3, A8              ; acc = acc + 2*x*h
        SUB     A1, 2, A1               ; A1-- (one 16-bit element)
        SUB     B0, 1, B0
 [B0]   B       filt
        NOP     5

* Sauvegarde du resultat dans la case "out" puis sortie de y(n) dans outfir.dat
        SHR     A8, 16, A8              ; y(n) = acc >> 16   (C54: STH A,*(out))
        MVKL    out, A5
        MVKH    out, A5
        STW     A8, *A5                 ; y(n) --> *out

* Retour au debut de la boucle sans fin   <-- PROBE POINT: out -> outfir.dat
        B       debut
        NOP     5

* Autre filtre: 794,1082,-4324,-8214,3530,14264,3530,-8214,-4324,1082,794
* (11 coefficients: N .set 11 and adr_coef .short 794,1082,...)
