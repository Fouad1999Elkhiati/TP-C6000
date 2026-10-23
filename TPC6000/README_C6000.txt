TP DSP - TMS320C5416 sources ported to the TMS320C6000 (C6713 DSK)
====================================================================

Target   : TMS320C6713 DSK (Spectrum Digital), AIC23 codec, 4 DIP switches,
           4 LEDs. Code Composer Studio 3.x projects (.pjt), as the originals.
Libraries: dsk6713bsl.lib, csl6713.lib, rts6700.lib (installed with the DSK).
Projects : firmacd2, sampling_bits, sampling_rate, fir_windows, IIR_order.

The filters and algorithms are unchanged
----------------------------------------
- All filter coefficients (FIR windows, monfiltre, IIR 1st/2nd order,
  firmacd) are the original values. Only the C type changed:
  "signed int" (16 bits on the C54) -> "short" (16 bits on the C6000).
- Integer sizes are preserved everywhere:
      C54 int (16 bits)  -> C6000 short / Int16
      C54 long (32 bits) -> C6000 int / Int32   (a C6000 long is 40 bits)
  so every shift, truncation, wrap-around and saturation behaves the same.
- The assembly routines (FIR_filters_asm.asm, iir_filters_second_order_asm.asm,
  firmacd.asm) were rewritten in C6000 assembly instruction by instruction:
      C54 MAC with FRCT=1, OVM=1  ->  SMPY + SADD (2*h*x, 32-bit saturation)
      C54 MACR                    ->  same + SADD 0x8000 (rounding)
      C54 MACD / DELAY            ->  LDH + STH loop (delay line)
      C54 SFTA A,-16              ->  SHR 16
  Only C62x instructions are used, so they also run on C6416/C6455/C674x
  (with that board's own BSL and project settings).
- Same sampling rate: the AIC23 is set to 48 kHz (the coefficients were
  designed for Fs = 48000 Hz; switches.c still counts 4800 samples = 0.1 s).

What changed (board and tools only)
-----------------------------------
- Codec: PCM3002 (C5416 DSK) -> AIC23 (C6713 DSK). Same read-left /
  write-left / read-right / write-right loop. Line in and headphone out at
  0 dB. The analog gains of the two boards are not identical, so absolute
  levels on the oscilloscope can differ a little; the digital processing
  does not.
- DIP switches and LEDs: DSK5416_DIP_get / DSK5416_LED_xxx ->
  DSK6713_DIP_get / DSK6713_LED_xxx (same switch numbering).
- No DSP/BIOS: on the C5416, UserTask() was started by DSP/BIOS (.cdb file).
  Here main() initialises the board and calls UserTask() directly. The .cdb,
  xxxcfg.* files are replaced by one linker file, C6713dsk.cmd.
- One memory space on the C6000 (no PAGE 0 program / PAGE 1 data), byte
  addresses (one 16-bit sample = 2 bytes).
- In IIR_order.c, switch_value is now initialised to 0 (the original read it
  before setting it on the very first sample). In FIR_windows.c the include
  names match the real file names (case) and the asm init function name in
  FIR_filters_asm.h is fixed (initialize).
- The FIR C function FIR_filter() is kept exactly as written, including its
  circular-buffer indexing (useful for question 15).

Using the projects (CCS 3.x)
----------------------------
Project > Open > <project>.pjt, then Project > Rebuild All, File > Load Program
(Debug\<project>.out), Debug > Run. If CCS was not installed in C:\CCStudio_v3.3,
check the include/library paths in Project > Build Options (they use
$(Install_dir)\C6000\dsk6713\include and \lib).

Changes to the lab handout steps on the C6000
---------------------------------------------
Part 1 (firmacd2):
- No "Program"/"Data" page choice in View > Memory: just type adr_coef or
  adr_debut_dat.
- Probe points: the two lines marked "PROBE POINT" in firmacd.asm
  (start of the loop: infir.dat -> in ; "B debut": out -> outfir.dat).
  File I/O: Address = in / out, Length = 1 (one 32-bit word on the C6000).
- Graph: Start Address = out, DSP Data Type = 32-bit signed integer
  (or 16-bit signed), Display Data Size = 40.
- File > Data > Load of outfir.dat: use a free internal RAM address such as
  0x00028000 instead of 0x8000.
- The C54 architecture questions (buses, accumulators, multiplier) obviously
  refer to the C54; the C6000 equivalents are in the C6000 CPU reference
  (SPRU189 / SPRU733).
- outfir_C54_reference.dat is the output recorded on the C5416 (kept for
  comparison).

Part 2 (fir_windows): the probe point on &mono_input with Length = 1 works
as before (mono_input is a 32-bit word so the probe cannot overwrite the
next variable); graphs on &left_output with DSP Data Type = 16-bit signed.

Verification done for this port
-------------------------------
- C code: each original program, compiled with C54 integer sizes (16-bit int,
  32-bit long), and its C6000 port were run with the same input signal
  (including full-scale bursts) over all 16 switch positions: identical codec
  output samples, LED changes and messages (480 000 samples per project).
- Assembly: the C6000 routines were run on an instruction-level C62x model
  (delay slots, saturation) and compared with the C54 instruction semantics
  (40-bit accumulator, FRCT, OVM, MACR): 86 400 outputs, all identical,
  including saturating inputs. firmacd reproduces outfir.dat recorded on the
  C5416.
- Not done: building with the TI C6000 tools and running on a real C6713 DSK.

Running WITHOUT a board (CCS simulator)
---------------------------------------
- firmacd2: runs on the simulator as it is (no board needed). Use probe
  points / File I/O with infir.dat as in the handout.
- fir_windows, IIR_order, sampling_bits, sampling_rate: open the
  <project>_sim.pjt file instead of <project>.pjt. It builds the SAME
  program files with sim\sim_board.c instead of the DSK board library:
    * the codec reads a test signal (impulse, step, sine, noise, sweep),
    * the outputs are stored in the arrays sim_left[] and sim_right[]
      (input in sim_in[]), 512 samples,
    * the DIP switches are the variable sim_switches,
    * the LEDs are the bits of sim_leds,
    * at the end the results are written to sim_output.csv (columns
      n,input,left,right, for Excel/Matlab), a summary is printed in Stdout,
      and the program stops.
  Settings at the top of sim\sim_board.c: sim_switches (filter 0..15),
  sim_signal (0 impulse, 1 step, 2 sine, 3 noise, 4 sweep), sim_amplitude,
  sim_freq_hz. Change them and Rebuild, or change them in a Watch window
  after Load Program and before Run.
  Graphs: View > Graph > Time/Frequency, Start Address = sim_left (or
  sim_right, sim_in), Acquisition Buffer Size = 512, Display Data Size = 512,
  DSP Data Type = 16-bit signed integer. Display Type = FFT Magnitude for the
  frequency response (with sim_signal = 0, impulse).
  In fir_windows, left = assembly filter, right = C filter (switches 1..5).
  In IIR_order, left = direct form I, right = direct form II.
- CCS 3.3 setup: Setup CCStudio > "C6713 Device Cycle Accurate Simulator,
  Little Endian" (Family C67xx, Platform simulator). Save & Quit, start CCS,
  Project > Open <project>_sim.pjt, Project > Rebuild All, File > Load
  Program (DebugSim\<project>_sim.out), Debug > Run (F5). The run takes a
  few seconds: 5000 silent samples first (the switch debounce of
  switches.c), then the 512 test samples.
- CCS v5/v6: File > Import > Legacy CCSv3.3 Projects, then a target
  configuration with a C6713 (or C67xx) simulator if your version lists one.

Testing on a PC (no board, no simulator) - folder pc_test
---------------------------------------------------------
Recent CCS versions (CCS 12, CCS 20 ...) cannot open the .pjt projects and
have no simulator. To test the algorithms, build them on the PC with gcc
(Linux or WSL Ubuntu on Windows):

    cd TPC6000/pc_test
    make                              # builds the 4 programs
    ./fir_windows 2 0                 # switches = 2, signal 0 = impulse
    ./IIR_order 4 2 1000              # switches = 4, sine at 1000 Hz
    python3 plot.py                   # sim_output.csv -> sim_output.png

  Arguments: <switches 0..15> <signal> [sine frequency in Hz]
  signal: 0 impulse, 1 step, 2 sine, 3 white noise, 4 sweep 0 -> 24 kHz.
  Needs: sudo apt install build-essential python3-numpy python3-matplotlib

  The program files of the lab are compiled as they are, with sim/sim_board.c
  as the board. The PC cannot execute C6000 assembly, so asm_models.c
  replaces the .asm routines with C functions that give exactly the same
  results (checked against the C54 and C6000 instruction behaviour).
