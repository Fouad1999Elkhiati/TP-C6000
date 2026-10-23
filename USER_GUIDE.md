# TP DSP on the C6000: user guide

This guide explains every command you can run in this repository: what it does, when to use it and what you get. It assumes you work in **WSL Ubuntu** in the folder `~/TP-C6000/TPC6000`.

---

## 1. What is in the repository

| Folder / file | What it contains | Used for |
|---|---|---|
| `firmacd2/` | 16-tap FIR in assembly, `infir*.dat` test files | Part 1 of the lab, in CCS only (simulator or board) |
| `sampling_bits/` | Program that reduces the number of bits | Quantisation tests |
| `sampling_rate/` | Program that reduces the sampling rate | Aliasing tests |
| `fir_windows/` | FIR filters (5 windows), C and assembly | FIR filter tests |
| `IIR_order/` | IIR filters of order 1 to 5, direct forms I and II | IIR filter tests |
| `sim/` | Simulated board (codec, switches, LEDs) | Running without a board |
| `pc_test/` | `Makefile`, `plot.py`, test driver | **All the commands of this guide** |
| `README_C6000.txt` | Notes on the C54 to C6000 port | Reference |
| `*.pjt`, `*_sim.pjt` | Code Composer Studio 3.x projects | CCS (board / simulator), see section 8 |

> The files ending in `:Zone.Identifier` were added by Windows when you downloaded the zip. They are harmless. To remove them:
> `find ~/TP-C6000 -name '*:Zone.Identifier' -delete`

**How it works.** The commands in `pc_test` compile the lab programs **unchanged** on your PC, together with a *simulated board* (`sim/sim_board.c`). The simulated codec feeds a test signal to the program, records the left and right outputs, writes them to a CSV file and stops. `plot.py` turns that CSV file into a picture.

---

## 2. One-time setup

Run once in WSL:

```bash
sudo apt update
sudo apt install build-essential python3-numpy python3-matplotlib
```

| Package | Why |
|---|---|
| `build-essential` | `gcc` and `make`, to compile the programs |
| `python3-numpy`, `python3-matplotlib` | needed by `plot.py` to draw the graphs |

---

## 3. Quick start (5 commands)

```bash
cd ~/TP-C6000/TPC6000/pc_test
make                       # build the 4 programs
./fir_windows 2 0          # Hamming high-pass filter, impulse input
python3 plot.py            # draw sim_output.csv -> sim_output.png
explorer.exe sim_output.png   # open the picture in Windows
```

---

## 4. Command reference

All commands are run **inside `pc_test/`**.

### 4.1 `make`: build the programs

| Command | What it does | When to use it |
|---|---|---|
| `make` | Builds all 4 programs: `fir_windows`, `IIR_order`, `sampling_bits`, `sampling_rate` | First time, and after **any** change to a `.c` or `.h` file, or to `sim/sim_board.c` |
| `make fir_windows` | Builds only that program (same for `IIR_order`, `sampling_bits`, `sampling_rate`) | To rebuild faster after editing one project |
| `make clean` | Deletes the 4 programs, `sim_output.csv` and `sim_output.png` | To start again from scratch |

- `make: Nothing to be done` / `is up to date` is normal: nothing changed since the last build.
- `make clean` does **not** delete CSV/PNG files you renamed (for example `lp_switch6.csv`) or folders you created.

### 4.2 `./<program> <switches> <signal> [frequency]`: run a test

```
./fir_windows    <switches> <signal> [frequency]
./IIR_order      <switches> <signal> [frequency]
./sampling_bits  <switches> <signal> [frequency]
./sampling_rate  <switches> <signal> [frequency]
```

| Argument | Meaning | Values |
|---|---|---|
| `switches` | The DIP switch value: it selects the filter or the processing, exactly like on the board (tables in section 5) | `0` to `15` |
| `signal` | The test signal fed to the codec | `0` impulse, `1` step, `2` sine, `3` white noise, `4` sweep 0 to 24 kHz |
| `frequency` | Sine frequency in Hz (only for signal `2`) | e.g. `500`, `1000`, `3000`; default `1000` |

**Which signal for which purpose**

| Signal | What it is | Use it to see |
|---|---|---|
| `0` impulse | one sample of 16384 (0x4000), then zeros | the **impulse response**, and with the FFT the **frequency response** of a filter |
| `1` step | 16384 held constant | the **step response**: rise time, overshoot, final (DC) value |
| `2` sine | a pure tone at `frequency` Hz | the **gain at one frequency**; **aliasing** (sampling_rate); **quantisation** (sampling_bits) |
| `3` white noise | random values | all frequencies at once, with a random signal |
| `4` sweep | a tone gliding from 0 to 24 kHz | how the output changes with frequency, in time |

**What the program does**
1. Prints the program title and the meaning of the switch value.
2. Runs 5000 silent samples (the time the switch-reading code needs to accept the value), then the 512 test samples.
3. Prints a summary (`SIMULATION DONE`, peak values, LEDs) and writes **`sim_output.csv`** in the current folder, then stops.

**`sim_output.csv` columns**

| Column | Content |
|---|---|
| `n` | sample number 0 to 511 (one sample = 1/48000 s = 20.8 µs) |
| `input` | test signal |
| `left` | left output of the program |
| `right` | right output of the program |

What *left* and *right* mean depends on the program (see the tables in section 5).

> **Careful:** the switch value is not checked. Only its 4 lowest bits are used, so `16` behaves like `0` and `99` like `3`. Always use `0` to `15`.
> Each run **overwrites** `sim_output.csv`. Rename it (section 4.4) to keep it.

Running a program without arguments prints its usage:

```bash
./fir_windows
# usage: ./fir_windows <switches 0..15> <signal> [sine frequency Hz]
```

### 4.3 `python3 plot.py [file.csv]`: draw the results

| Command | What it does |
|---|---|
| `python3 plot.py` | Reads `sim_output.csv`, writes `sim_output.png` |
| `python3 plot.py lp_hamming.csv` | Reads that file, writes `lp_hamming.png` (same name, `.png`) |
| `python3 plot.py results/test1.csv` | Works with files in sub-folders too |

The picture has two graphs:
- **Top, time domain:** input (light), left output (solid) and right output (dashed) against the sample number.
- **Bottom, frequency domain:** the spectrum (FFT) of each signal in dB, from 0 to 24 kHz. With the **impulse** input (`signal 0`), the spectrum of an output is the **frequency response** of the filter.

### 4.4 Viewing and keeping results

| Command | What it does |
|---|---|
| `explorer.exe sim_output.png` | Opens the picture with the Windows image viewer |
| `explorer.exe .` | Opens the current folder in Windows Explorer (see all PNG/CSV files) |
| `mv sim_output.csv name.csv` | Keeps a result under a new name before the next run overwrites it |
| `mkdir -p results` | Creates a folder to collect results |

The CSV files open directly in Excel (double-click in Explorer) or in Matlab (`readmatrix('name.csv')`).

---

## 5. Programs, switch values and experiments

### 5.1 `sampling_bits`: number of bits (quantisation)

Left and right: the input with fewer bits (both channels identical).

| Switch | Bits kept | Switch | Bits kept |
|---|---|---|---|
| 0 | 16 (unchanged) | 6 | 10 |
| 1 | 15 | 7 | 9 |
| 2 | 14 | 8 | 8 |
| 3 | 13 | 9 | 7 |
| 4 | 12 | 10 | 6 |
| 5 | 11 | 11 | 5 |
| 12 to 15 | not used (output not updated) | | |

**Experiments**

```bash
./sampling_bits 0 2 1000  && python3 plot.py    # 16 bits: clean sine
./sampling_bits 8 2 1000  && python3 plot.py    # 8 bits: small steps
./sampling_bits 11 2 1000 && python3 plot.py    # 5 bits: visible staircase
```

What to look for: the output becomes a staircase; in the spectrum the noise floor rises by about **6 dB per bit removed** (SNR ≈ 6.02·N + 1.76 dB).

### 5.2 `sampling_rate`: sampling rate (aliasing)

Left and right: the input, held so that only one sample out of k is kept (both identical).

| Switch | Fs' | Switch | Fs' |
|---|---|---|---|
| 0 | 48 kHz | 7 | 6.0 kHz |
| 1 | 24 kHz | 8 | 5.3 kHz |
| 2 | 16 kHz | 9 | 4.8 kHz |
| 3 | 12 kHz | 10 | 4.0 kHz |
| 4 | 9.6 kHz | 11 | 3.0 kHz |
| 5 | 8.0 kHz | 12 | 2.0 kHz |
| 6 | 6.8 kHz | 13 to 15 | not used |

**Experiments**

```bash
./sampling_rate 12 2 500  && python3 plot.py    # 500 Hz < Fs'/2 = 1 kHz: correct, just a staircase
./sampling_rate 12 2 3000 && python3 plot.py    # 3000 Hz > 1 kHz: comes out at |3000 - 2*2000| = 1000 Hz
./sampling_rate 9 2 3000  && python3 plot.py    # Fs' = 4.8 kHz: comes out at |3000 - 4800| = 1800 Hz
./sampling_rate 12 4      && python3 plot.py    # sweep: the output frequency goes up and down (folding)
```

What to look for: above Fs'/2 the output frequency is the **alias** |f − k·Fs'|. Read it on the main peak of the bottom graph.

### 5.3 `fir_windows`: FIR filters with windows

| Switch | Left output | Right output |
|---|---|---|
| 0 | input ÷ 2 (no filter) | input ÷ 2 |
| 1 | 1200 Hz **high pass**, rectangular window, **assembly** | same filter in **C** |
| 2 | high pass, Hamming, assembly | same, C |
| 3 | high pass, Hanning, assembly | same, C |
| 4 | high pass, Blackman, assembly | same, C |
| 5 | high pass, "Kaiser" (*same coefficients as rectangular in the lab files*), assembly | same, C |
| 6 | 1200 Hz **low pass**, rectangular (dual filter, assembly) | same as left |
| 7 | low pass, Hamming | same as left |
| 8 | low pass, Hanning | same as left |
| 9 | low pass, Blackman | same as left |
| 10 | low pass, "Kaiser" (= rectangular) | same as left |
| 11 | high pass ÷ 2 | low pass (crossover) |
| 12 to 15 | input ÷ 2 (free for your own filter, lab question 19) | input ÷ 2 |

**Experiments**

```bash
# Frequency response of one filter (impulse input):
./fir_windows 2 0 && python3 plot.py

# Compare the 4 windows in low pass, keeping each result:
for s in 6 7 8 9; do ./fir_windows $s 0 > /dev/null && mv sim_output.csv lp_switch$s.csv && python3 plot.py lp_switch$s.csv; done
explorer.exe .

# Assembly (left) vs C (right): same gain, the C output is 2 samples later
./fir_windows 2 0 && python3 plot.py

# Crossover: low pass on one side, high pass on the other
./fir_windows 11 0 && python3 plot.py

# Gain at one frequency (the lab's sine method, question 11):
./fir_windows 9 2 500  && python3 plot.py    # in the passband: output ≈ input
./fir_windows 9 2 5000 && python3 plot.py    # in the stopband: output almost zero
```

What to look for: all filters are at −6 dB at 1200 Hz; the side lobes of the low pass drop from rectangular (about −31 dB) to Blackman (about −66 dB); the impulse response is centred on sample 25 (delay of a 51-tap linear-phase FIR).

### 5.4 `IIR_order`: IIR filters of order 1 to 5

Left = **direct form I**, right = **direct form II** (same filter, two structures), except for switches 11 to 15.

| Switch | Filter (cutoff 2000 Hz) |
|---|---|
| 0 | no filter |
| 1 / 2 / 3 / 4 / 5 | **low pass** of order 1 / 2 / 3 / 4 / 5 |
| 6 / 7 / 8 / 9 / 10 | **high pass** of order 1 / 2 / 3 / 4 / 5 |
| 11 to 15 | crossover of order 1 to 5: left = low pass, right = high pass (mono input) |

**Experiments**

```bash
# Frequency response, order 1 then order 4 (compare the slopes):
./IIR_order 1 0 && mv sim_output.csv iir_lp1.csv && python3 plot.py iir_lp1.csv
./IIR_order 4 0 && mv sim_output.csv iir_lp4.csv && python3 plot.py iir_lp4.csv

# Step response: overshoot of order 2 and 4
./IIR_order 2 1 && python3 plot.py
./IIR_order 4 1 && python3 plot.py

# High pass step: output returns to 0
./IIR_order 7 1 && python3 plot.py

# Direct form I (left) vs II (right): compare the noise floor in the spectrum
./IIR_order 2 2 1031 && python3 plot.py
```

What to look for: each order adds about 6 dB per octave of slope; order 2 overshoots by about 4 % on a step; the right channel (direct form II) has a higher noise floor.

> For IIR filters, the impulse method shows small deviations at very low levels (a −12 LSB offset caused by truncation). For accurate gains use sines (`signal 2`) at several frequencies.

---

## 6. Changing the test settings

The settings that are not command-line arguments are at the top of **`sim/sim_board.c`**. Edit them with any editor (for example `nano ../sim/sim_board.c`), save, then run `make` again.

| Setting | Default | Meaning |
|---|---|---|
| `sim_amplitude` | `0x4000` (16384, half scale) | amplitude of the impulse, step, sine, noise and sweep. Use `32000` for near full scale (quantisation tests), smaller values to see low-level effects |
| `#define SIM_N` | `512` | number of samples recorded (more = finer frequency resolution, 48000/SIM_N Hz) |
| `#define SIM_WARMUP` | `5000` | silent samples before the test (must stay above 4801 for the switch value to be accepted) |

`sim_switches`, `sim_signal` and `sim_freq_hz` also appear there, but the command-line arguments replace them.

---

## 7. Useful recipes

**Keep all the results of a session in one folder**

```bash
mkdir -p results
./fir_windows 7 0 > /dev/null && mv sim_output.csv results/fir_lp_hamming.csv
python3 plot.py results/fir_lp_hamming.csv          # -> results/fir_lp_hamming.png
```

**Measure the gain at several frequencies (sine method)**

```bash
for f in 250 500 1000 2000 4000 8000; do
  ./IIR_order 2 2 $f > /dev/null && mv sim_output.csv iir2_${f}Hz.csv && python3 plot.py iir2_${f}Hz.csv
done
```

Compare the output amplitude (top graph) with the input amplitude for each frequency, as you would on an oscilloscope.

**See every switch value of a program**

```bash
for s in $(seq 0 15); do ./sampling_rate $s 2 3000 > /dev/null && mv sim_output.csv rate_s$s.csv; done
```

**Silence the program messages**: add `> /dev/null` after the command; the CSV file is still written.

---

## 8. Without the PC tools: Code Composer Studio

| You have | Open | Notes |
|---|---|---|
| CCS 3.3 + C6713 DSK board | `<project>/<project>.pjt` | Real codec, DIP switches and LEDs |
| CCS 3.3, no board (simulator) | `<project>/<project>_sim.pjt` | Uses `sim/sim_board.c`; settings in that file; graphs on `sim_in`, `sim_left`, `sim_right` (16-bit signed, size 512) |
| CCS 3.3, `firmacd2` | `firmacd2/firmacd2.pjt` | Works on the board or the simulator; probe points on the lines marked `PROBE POINT` in `firmacd.asm` with `infir.dat` |
| CCS 4 to 6 | File → Import → Legacy CCSv3.3 Projects | Then create a target configuration |
| CCS 12 / CCS 20 | — | Cannot open `.pjt` and has no simulator: use `pc_test` |

`firmacd2` (the assembly-only program) cannot run in `pc_test`, because a PC cannot execute C6000 assembly. Its results are shown in the lab report (Figure 1).

---

## 9. Troubleshooting

| Problem | Cause | Fix |
|---|---|---|
| `make: command not found` or `gcc: not found` | build tools missing | `sudo apt install build-essential` |
| `ModuleNotFoundError: No module named 'numpy'` / `'matplotlib'` | Python libraries missing | `sudo apt install python3-numpy python3-matplotlib` |
| `No such file or directory: 'sim_output.csv'` when plotting | no test was run in this folder, or it was renamed | run a program first, or give the file name to `plot.py` |
| `./fir_windows: No such file or directory` | not built yet, or not in `pc_test/` | `cd ~/TP-C6000/TPC6000/pc_test && make` |
| `usage: ...` printed | missing arguments | give at least `<switches> <signal>` |
| The output did not change after editing a file | program not rebuilt | `make` |
| Wrong filter used | switch value above 15 | use 0 to 15 only |
| `explorer.exe` does nothing | WSL Windows interop disabled | open `\\wsl$\Ubuntu\home\<you>\TP-C6000\TPC6000\pc_test` in Windows Explorer |
| Graph looks empty with signal `2` at a high switch value of `sampling_bits` / `sampling_rate` | switches 12 to 15 (or 13 to 15) are "not used": the output is never updated | use a switch value from the tables |
