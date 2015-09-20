/*
 * spi.h
 *
 * Created: 05-Aug-15 12:44:22 AM
 *  Author: emon1
 */ 

#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#define SPI_DDR		DDRB
#define SPI_PORT	PORTB

#define MOSI_DDR	DDRB
#define MOSI_PORT	PORTB
#define MOSI_BIT	3

#define MISO_DDR	DDRB
#define MISO_PORT	PORTB
#define MISO_BIT	4

#define SCK_DDR		DDRB
#define SCK_PORT	PORTB
#define SCK_BIT		5


/**
 * initialize the SPI module as master
 */
inline void spi_master_initialize(){
	//set !SS,MOSI,SCK pin as output pins
	SPI_DDR |= (1<<PINB2) | (1<<MOSI_BIT) | (1<<SCK_BIT);
	//set MISO as input pin
	SPI_DDR &= ~(1<<MISO_BIT);
	//enable SPI and set as master
	SPCR |= (1<<SPE) | (1<<MSTR);
}

/**
 * SPI master trade pass by value
 * @param data value of data byte to be sent
 * return data sent back from the salve
 */
inline uint8_t spi_master_transmit_byte_val(uint8_t data){
	//fill SPDR with data to write
	SPDR = data;
	//wait for transmission to complete
	while(!(SPSR&(1<<SPIF)));
	return SPDR;
}
/**
 * SPI master trade pass by reference
 * @param data address of data byte to be sent
 * return data sent back from the salve
 */
inline uint8_t spi_master_transmit_byte_ref(uint8_t *data){
	//fill SPDR with data to write
	SPDR = *data;
	//wait for transmission to complete
	while(!(SPSR&(1<<SPIF)));
	return SPDR;
}




extern volatile int us;

inline int millis(){
	
	if(us > 0){
		asm("nop");
	}
	return us;
	//return 0;
}

inline void initialize_timer_0A(){
	TCCR0A |= (1<<WGM01) | (1<<WGM00); //fast pwm
	TCCR0B |= (1<<FOC0A) ;//force output compare match on channel A
	TCCR0B |= (1<<CS01); // div by 8 , therefore , if 8MHz --> 1Mhz ....if 16 MHz --> 2 MHz
	
	OCR0A = 10; //if 8 Mhz return 10 us on compare,  if 16 Mhz return 5 us on compare
	TIMSK0 |= (1<<OCIE0A);
	sei();
	
}


#endif /* HARDWARE_H_ */