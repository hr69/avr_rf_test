/*
 * nrf24l01p.cpp
 *
 * Created: 19-Sep-15 2:30:28 AM
 *  Author: emon1
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "serial_stdio.h"

#include "RF24-master/RF24.h"


int main(void)
{
	
	usart_set_baud_rate(9600);
	usart_setup(0,0,0,3,0);
	usart_enable();
	stdio_serial_initialize();

	RF24 radio;


	radio.begin();
	
    while(1)
    {
        //TODO:: Please write your application code 
		
    }
}




