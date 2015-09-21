/*
 * hardware.cpp
 *
 * Created: 21/09/2015 02:50:55
 *  Author: prathibha
 */ 

#include "hardware.h"

volatile int us_4 = 0;
volatile int us_counter = 0;
volatile int ms_counter = 0;
volatile int elapsed_us = 0;
volatile int elapsed_ms = 0;

ISR (TIMER0_OVF_vect){
	us_4++;
	if(us_4>=250){  ////if 8 Mhz make it >100 ,  if 16 Mhz   make it  > 200
		us_4 = 0;
		elapsed_us ++;  //increment the us
		us_counter++;
		if(us_counter>1000){
			us_counter=0;
			elapsed_ms++; //increment the ms
			ms_counter++;
			if(ms_counter>=1000)
			{
				ms_counter = 0;
				PORTB^=1;
			}
		}
		asm("nop");
	}
}