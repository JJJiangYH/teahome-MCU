#ifndef __LED_H_
#define __LED_H_

#include "delay.h"

/**********************
���ű�������
***********************/			
sbit LED_R=P0^7;		 //��ɫLED��IO��P07	
sbit LED_B=P0^6;     //��ɫLED��IO��P06

extern void led_on(uint8 led_idx);
extern void led_off(uint8 led_idx);
extern void led_toggle(uint8 led_idx);
extern void leds_on(void);
extern void leds_off(void);

#endif
