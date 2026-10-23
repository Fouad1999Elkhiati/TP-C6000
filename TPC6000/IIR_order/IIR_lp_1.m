% IIR_LP_1.m
% Calculating IIR filter coefficients of 2nd Order IIR Filter 

% Numerator coefficients

B0 = 2015;
B1 = 2015 ;
B2 = 0;

% Denominator coefficients

A0 = 32767;        % Always 1.0000
A1 = -28736;
A2 = 0;

b = [B0 B1 B2];
a = [A0 A1 A2];

Fs = 48000          % Sampling frequency 
[H,f] = freqz ( b, a, 512, Fs);
magnitude = 20 * log10(abs(H));
plot ( f, magnitude), grid on
xlabel ('Frequency (Hz)');
ylabel ('Magnitude Response (dB)');
title ('1000 Hz First Order IIR Low Pass Filter Using BLT. 48000 Hz Sampling');
set (gca, 'xlim', [0, 12000])
set (gca, 'XTick', [0, 2000, 4000, 6000, 8000, 10000, 12000])
set (gca, 'XTickLabel', [0 2000 4000 6000 8000 10000, 12000])
set (gca, 'ylim', [-42, 6])
set (gca, 'YTick', [-42, -36, -30, -24, -18, -12, -6, 0, 6])
set (gca, 'YTickLabel', [-42, -36,-30, -24, -18, -12, -6, 0, 6])
%zplane ( b, a)