% fir_len_1.m
% Calculating FIR coefficients for different coefficient lengths

Fs = 48000;          % Sampling frequency 
FN = Fs/2;           % Nyquist frequency 
Fc = 2000/FN;        % Cutoff frequency normalised to FN
N = 51;              % 51 coefficients
hd = fir1 ( N-1, Fc, 'low', boxcar(N), 'noscale');
wn = hamming(N);
hn = fir1 ( N-1, Fc, 'low', wn, 'noscale' ); % Calculate blackman window coefficients
[H,f] = freqz ( hn, 1, 512, Fs);
magnitude = 20 * log10(abs(H));
plot ( f, magnitude), grid on
xlabel ('Frequency (kHz)');
ylabel ('Magnitude Response (dB)');
title ('2000 Hz Low Pass Filter Using Blackman Window. 51 Coefficients. 48000 Hz Sampling');
set (gca, 'xlim', [0, 4000])
set (gca, 'XTick', [0, 1000, 2000, 3000, 4000])
set (gca, 'XTickLabel', [0 1 2 3 4])
set (gca, 'ylim', [-42, 6])
set (gca, 'YTick', [-42, -36, -30, -24, -18, -12, -6, 0, 6])
set (gca, 'YTickLabel', [-42, -36, -30, -24, -18, -12, -6, 0, 6])
