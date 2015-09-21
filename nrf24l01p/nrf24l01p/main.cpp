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
#include "RF24-master/arch/avr/hardware.h"

#include "RF24-master/RF24.h"

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 0;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
/**********************************************************/

uint8_t addresses[][6] = {"1Node","2Node"};

// Used to control whether this node is sending or receiving
bool role = 0;

int main(void)
{

	//UART
	usart_set_baud_rate(9600);
	usart_setup(0,0,0,3,0);
	usart_enable();
	stdio_serial_initialize();
	
	printf("RF24/examples/GettingStarted\n");
	printf("*** PRESS 'T' to begin transmitting to the other node\n");

	initialize_timer_0A();

    radio.begin();
	//radio.setDataRate(RF24_250KBPS);
	radio.setPALevel(RF24_PA_LOW);
	
  // Open a writing and reading pipe on each radio, with opposite addresses
	if(radioNumber) {
		radio.openWritingPipe(addresses[1]);
		radio.openReadingPipe(1,addresses[0]);
	}
	else {
		radio.openWritingPipe(addresses[0]);
		radio.openReadingPipe(1,addresses[1]);
	}
    
    radio.startListening();
    
	
	
    while(1) {
		
        /****************** Ping Out Role ***************************/
        if (role == 1)  {
	        
	        radio.stopListening();                                    // First, stop listening so we can talk.
	        
	        
	        printf("Now sending\n");

	        unsigned long time = 123456;                             // Take the time, and send it.  This will block until complete
	        if (!radio.write( &time, sizeof(unsigned long) )){
		        printf("failed");
	        }
	        
	        radio.startListening();                                    // Now, continue listening
	        
	        unsigned long started_waiting_at = millis();               // Set up a timeout period, get the current microseconds
	        bool timeout = false;                                   // Set up a variable to indicate if a response was received or not
	        
	        while ( ! radio.available() ){                             // While nothing is received
		        if (millis() - started_waiting_at > 200 ){            // If waited longer than 200ms, indicate timeout and exit while loop
			        timeout = true;
			        break;
		        }
	        }
	        
	        if ( timeout ){                                             // Describe the results
		        printf("Failed, response timed out.\n");
		        } else {
		        unsigned long got_time;                                 // Grab the response, compare, and send to debugging spew
		        radio.read( &got_time, sizeof(unsigned long) );
		        unsigned long time = millis();
		        
		        // Spew it
		        printf("Sent ");
		        printf("%ld", time);
		        printf(", Got response ");
		        printf("%ld", got_time);
		        printf(", Round-trip delay ");
		        printf("%ld", time-got_time);
		        printf(" microseconds\n");
	        }

	        // Try again 1s later
	        _delay_ms(1000);
        }

		/****************** Pong Back Role ***************************/

		if ( role == 0 )
		{
			unsigned long got_time;
	
			if( radio.available()){
				// Variable for the received timestamp
				while (radio.available()) {                                   // While there is data ready
					radio.read( &got_time, sizeof(unsigned long) );             // Get the payload
				}
		
				radio.stopListening();                                        // First, stop listening so we can talk
				radio.write( &got_time, sizeof(unsigned long) );              // Send the final one back.
				radio.startListening();                                       // Now, resume listening so we catch the next packets.
				printf("Sent response ");
				printf("%ld\n", got_time);
			}
		}

		/****************** Change Roles via Serial Commands ***************************/
		if ( bit_is_set(UCSR0A, RXC0) )
		{
			char buffer[12];
			char c = scanf("%12s", buffer);
			if ( c == 'T' && role == 0 ){
				printf("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK\n");
				role = 1;                  // Become the primary transmitter (ping out)
				
			}else
			if ( c == 'R' && role == 1 ){
				printf("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK\n");
				role = 0;                // Become the primary receiver (pong back)
				radio.startListening();
				
			}
		}
		
    }
}




