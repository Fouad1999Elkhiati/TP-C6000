% fir_window_1.m
% Calculating FIR coefficients for different window functions

Fs = 48000;          % Sampling frequency 
FN = Fs/2;           % Nyquist frequency 
Fc = 2000/FN;        % Cutoff frequency normalised to FN
N = 51;              % 51 coefficients
hd = fir1 ( N-1, Fc, 'low', boxcar(N), 'noscale');
wn = boxcar(N);
hn = fir1 ( N-1, Fc, 'low', wn, 'noscale' ); % Calculate blackman window coefficients
[H,f] = freqz ( hn, 1, 512, Fs);
magnitude = 20 * log10(abs(H));
plot ( f, magnitude), grid on
xlabel ('Frequency (kHz)');
ylabel ('Magnitude Response (dB)');
title ('2000 Hz Low Pass FIR Filter Using Rectangular Window. 48000 Hz Sampling');
set (gca, 'xlim', [0, 12000])
set (gca, 'XTick', [0 1000 2000 3000 4000 5000 6000 7000 8000 9000 10000 11000 12000])
set (gca, 'XTickLabel', [0 1 2 3 4 5 6 7 8 9 10 11 12])
set (gca, 'ylim', [-96, 6])
set (gca, 'YTick', [-96 -90 -84 -78 -72 -66 -60 -54 -48 -42, -36, -30, -24, -18, -12, -6, 0, 6])
set (gca, 'YTickLabel', [-96 -90 -84 -78 -72 -66 -60 -54 -48 -42, -36, -30, -24, -18, -12, -6, 0, 6])
