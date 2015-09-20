/*
 * hardware.cpp
 *
 * Created: 21/09/2015 02:50:55
 *  Author: prathibha
 */ 

#include "hardware.h"

volatile int us_10 = 0;
volatile int us;

ISR (TIMER0_COMPA_vect){
	us_10++;
	if(us_10>=100){  ////if 8 Mhz make it >100 ,  if 16 Mhz   make it  > 200
		us_10 = 0;
		us ++;
		asm("nop");
	}
}