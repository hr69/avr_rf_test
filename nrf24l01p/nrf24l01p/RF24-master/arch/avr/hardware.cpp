/*
 * hardware.cpp
 *
 * Created: 21/09/2015 02:50:55
 *  Author: prathibha
 */ 

#include "hardware.h"

volatile int us_10 = 0;
volatile int us_counter = 0;
volatile int ms_counter = 0;
volatile int elapsed_us = 0;
volatile int elapsed_ms = 0;

ISR (TIMER0_COMPA_vect){
	us_10++;
	if(us_10>=100){  ////if 8 Mhz make it >100 ,  if 16 Mhz   make it  > 200
		us_10 = 0;
		elapsed_us ++;
		us_counter++;
		if(us_counter++>1000){
			us_counter=0;
			elapsed_ms++;
		}
		asm("nop");
	}
}