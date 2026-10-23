"""plot.py - plot sim_output.csv: input, left and right outputs, and the
magnitude of their spectra (use the impulse signal for a frequency response).
Usage: python3 plot.py [sim_output.csv]   ->  sim_output.png"""
import sys
import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

name = sys.argv[1] if len(sys.argv) > 1 else "sim_output.csv"
d = np.loadtxt(name, delimiter=",", skiprows=1)
n, x, left, right = d[:, 0], d[:, 1], d[:, 2], d[:, 3]
fs = 48000.0

fig, ax = plt.subplots(2, 1, figsize=(10, 7))
ax[0].plot(n, x, label="input", alpha=0.5)
ax[0].plot(n, left, label="left")
ax[0].plot(n, right, "--", label="right")
ax[0].set_xlabel("sample n"); ax[0].set_ylabel("value"); ax[0].legend(); ax[0].grid(True)

f = np.fft.rfftfreq(len(n), 1 / fs)
for sig, lab in ((x, "input"), (left, "left"), (right, "right")):
    mag = 20 * np.log10(np.abs(np.fft.rfft(sig)) + 1e-9)
    ax[1].plot(f, mag, label=lab)
ax[1].set_xlabel("frequency (Hz)"); ax[1].set_ylabel("|FFT| (dB)"); ax[1].legend(); ax[1].grid(True)
ax[1].set_xlim(0, fs / 2)
fig.tight_layout()
fig.savefig(name.replace(".csv", ".png"), dpi=110)
print("saved", name.replace(".csv", ".png"))
