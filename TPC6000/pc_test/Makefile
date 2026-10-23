# PC test of the lab algorithms (no board, no CCS). Usage: make, then e.g.
#   ./fir_windows 2 0        (switch 2 = Hamming high pass, impulse)
#   python3 plot.py          (plots sim_output.csv)
CC     = gcc
CFLAGS = -O1 -fwrapv -std=gnu89 -w -Dmain=board_main -I../sim
COMMON = ../sim/sim_board.c driver.c asm_models.c -lm

all: fir_windows IIR_order sampling_bits sampling_rate

fir_windows: $(wildcard ../fir_windows/*.c) $(wildcard ../fir_windows/*.h) ../sim/sim_board.c driver.c asm_models.c
	$(CC) $(CFLAGS) -I../fir_windows -o $@ ../fir_windows/*.c $(COMMON)

IIR_order: $(wildcard ../IIR_order/*.c) $(wildcard ../IIR_order/*.h) ../sim/sim_board.c driver.c asm_models.c
	$(CC) $(CFLAGS) -I../IIR_order -o $@ ../IIR_order/*.c $(COMMON)

sampling_bits: $(wildcard ../sampling_bits/*.c) ../sim/sim_board.c driver.c asm_models.c
	$(CC) $(CFLAGS) -I../sampling_bits -o $@ ../sampling_bits/*.c $(COMMON)

sampling_rate: $(wildcard ../sampling_rate/*.c) ../sim/sim_board.c driver.c asm_models.c
	$(CC) $(CFLAGS) -I../sampling_rate -o $@ ../sampling_rate/*.c $(COMMON)

clean:
	rm -f fir_windows IIR_order sampling_bits sampling_rate sim_output.csv sim_output.png
