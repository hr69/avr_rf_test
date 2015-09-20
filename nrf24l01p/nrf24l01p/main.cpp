/*
 * nrf24l01p.cpp
 *
 * Created: 19-Sep-15 2:30:28 AM
 *  Author: emon1
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include "RF24-master/RF24.h"







int main(void)
{
	RF24 radio(9,10);

	radio.begin();
	
    while(1)
    {
        //TODO:: Please write your application code 
		
    }
}




