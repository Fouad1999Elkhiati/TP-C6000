/* dsk6713_led.h - SIMULATION version: LEDs = bits of variable sim_leds */
#ifndef DSK6713_LED_SIM_H
#define DSK6713_LED_SIM_H
void DSK6713_LED_init(void);
void DSK6713_LED_on(Uint32 ledNum);
void DSK6713_LED_off(Uint32 ledNum);
void DSK6713_LED_toggle(Uint32 ledNum);
#endif
