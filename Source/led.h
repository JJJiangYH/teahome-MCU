#ifndef __LED_H_
#define __LED_H_

#include "delay.h"

/**********************
引脚别名定义
***********************/			
sbit LED_R=P0^7;		 //红色LED用IO口P07	
sbit LED_B=P0^6;     //蓝色LED用IO口P06

extern void led_on(uint8 led_idx);
extern void led_off(uint8 led_idx);
extern void led_toggle(uint8 led_idx);
extern void leds_on(void);
extern void leds_off(void);

#endif
