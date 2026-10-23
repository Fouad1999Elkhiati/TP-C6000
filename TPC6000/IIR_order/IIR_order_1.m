% IIR_order_1.m
% Calculating IIR filter coefficients of 1st Order lowpass IIR Filter 

% Numerator coefficients for first order lowpass

B00 = 3812;
B01 = 3812;
B02 = 0;

% Denominator coefficients for first order filter

A00 = 32767;
A01 = -25143;
A02 = 0;

% Numerator coefficients for second order lowpass 

B10 = 472;
B11 = 472 * 2 ;
B12 = 472;

% Denominator coefficients for second order lowpass

A10 = 32767;        % Always 1.0000
A11 = -26754 * 2;
A12 = 22629;

sos = [B00 B01 B02 A00 A01 A02 ];

% Convert second order sections to transfer function
[b, a] = sos2tf( sos );

Fs = 48000          % Sampling frequency 
[H,f] = freqz ( b, a, 512, Fs);
magnitude = 20 * log10(abs(H));
plot ( f, magnitude), grid on
xlabel ('Frequency (Hz)');
ylabel ('Magnitude Response (dB)');
title ('2000 Hz First Order IIR Low Pass Filter Using BLT. 48000 Hz Sampling');
set (gca, 'xlim', [0, 12000])
set (gca, 'XTick', [0, 2000, 4000, 6000, 8000, 10000, 12000])
set (gca, 'XTickLabel', [0 2000 4000 6000 8000 10000, 12000])
set (gca, 'ylim', [-42, 6])
set (gca, 'YTick', [-42, -36, -30, -24, -18, -12, -6, 0, 6])
set (gca, 'YTickLabel', [-42, -36,-30, -24, -18, -12, -6, 0, 6])

figure
zplane ( b, a)