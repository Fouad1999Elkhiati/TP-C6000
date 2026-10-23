/* driver.c - PC test: ./<program> <switches 0..15> <signal 0..4> [sine Hz] */
#undef main
#include <stdio.h>
#include <stdlib.h>
extern volatile unsigned int sim_switches, sim_signal, sim_freq_hz;
void UserTask();
int main(int argc, char **argv)
{
    if (argc < 3) {
        printf("usage: %s <switches 0..15> <signal> [sine frequency Hz]\n"
               "signal: 0 impulse, 1 step, 2 sine, 3 white noise, 4 sweep 0-24 kHz\n", argv[0]);
        return 1;
    }
    sim_switches = (unsigned) atoi(argv[1]);
    sim_signal   = (unsigned) atoi(argv[2]);
    if (argc > 3) sim_freq_hz = (unsigned) atoi(argv[3]);
    UserTask();                 /* stops by itself (exit) after 512 samples */
    return 0;
}
