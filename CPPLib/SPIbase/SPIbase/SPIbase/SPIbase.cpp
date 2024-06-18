/*
 * SPIbase.cpp
 *
 *  Author: Adam Czaga czagaadam@gmail.com
 *
 *  SPI base class and inherited master and slave classes.
 * Slave class should be used for communication between two atmega, every other cases for sensors etc. master class should be used
 *
 */ 

#include <avr/pgmspace.h>
#include <avr/io.h>
#define F_CPU 16000000UL        //16Mhz
#include <avr/interrupt.h>		//cli sei
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SPIbase.h"
#include "GPIObase/GPIObase/GPIObase/ISRbase.h"

ISRbase<SPIbase> SPIbase::ISR_LIST;

SPIbase::SPIbase(){}

SPIbase::SPIbase(SPI_TypeDef SPI, SPI_SPEED speed)
{
	if(SPI == SP0)
	{	
		_MOSI = Dout(_SPI_PORT_0.PORT, _SPI_PORT_0.MOSI, GPIO_PIN_RESET);
		_MISO = Din(_SPI_PORT_0.PORT, _SPI_PORT_0.MISO);
		_SCK  = Dout(_SPI_PORT_0.PORT, _SPI_PORT_0.SCK, GPIO_PIN_RESET);
		_PORT = SPI;
		
	}
	_speed = speed;
	//#todo:
}

SPIbase::SPIbase(SPI_TypeDef SPI, SPI_SPEED speed, spi_isr_cb cb)
{
	if(SPI == SP0)
	{	
		_MOSI = Dout(_SPI_PORT_0.PORT, _SPI_PORT_0.MOSI, GPIO_PIN_RESET);
		// #todo: setting reset state in constructor not working
		_MOSI.write(GPIO_PIN_RESET);//????????????????????????????????????
		_MISO = Din(_SPI_PORT_0.PORT, _SPI_PORT_0.MISO);
		_SCK  = Dout(_SPI_PORT_0.PORT, _SPI_PORT_0.SCK, GPIO_PIN_RESET);
		_SCK.write(GPIO_PIN_RESET);//????????????????????????????????????		
		_PORT = SPI;
		_speed = speed;
		set_isr_cb(cb);
	}

	//#todo:
}

SPIbase::~SPIbase(void)
{
	
}
//#todo: implement timeout
//timeout cannot be implemented for SPI
uint8_t  SPIbase::SPI_tranceive (uint8_t data, uint8_t& receive)
{	// Load data into the buffer
	SPDR = data;
	
	//Wait until transmission complete
	while( ( !( SPSR & (1<<SPIF) ) ) );
	
	// Return received data
	receive = SPDR;
	
	//#todo: timeout
	return 0;
}
uint8_t  SPIbase::SPI_transmit (uint8_t data)
{	// Load data into the buffer
	SPDR = data;
	
	//Wait until transmission complete
	while( ( !( SPSR & (1<<SPIF) ) ) );
	
	//#todo: timeout
	return 0;
}
uint8_t SPIbase::SPI_transmit (uint8_t data[], uint8_t nbyte)
{
	//uint8_t retVal = 0x00;
	for (uint8_t i=0; i<nbyte ;i++ ) {
		//retVal = 
		SPI_transmit(data[i]);
	}

	//#todo: timeout
	return 0;
}
uint8_t SPIbase::SPI_receive (uint8_t data[], uint8_t *receive, uint8_t nbyte)
{
	if(nbyte<100)
	{
		uint8_t retVal[100];
		for (uint8_t i=0; i<nbyte;i++)
		{
			SPI_tranceive(data[i], retVal[i]);
		}
		memcpy(receive, retVal, nbyte);
	}
	else
	{
		memcpy(receive, 0x00, 1);
		//#todo: return error code
	}
	//#todo: timeout
	return 0;	
}
uint8_t SPIbase::SPI_receive (uint8_t& receive)
{
	// transmit dummy byte
	SPDR = 0x00;

	// Wait for reception complete
	while(!(SPSR & (1 << SPIF)));

	// return Data Register
	receive = SPDR;	
	
	//#todo: timeout
	return 0;	
}
uint8_t SPIbase::SPI_write_register (uint8_t command, uint8_t adress, uint8_t data )
{
	uint8_t retval = 1; 				
	retval = SPI_transmit(command);
	if(!retval) retval = SPI_transmit(adress);
	if(!retval) retval = SPI_transmit(data);
	return retval;	
}
uint8_t SPIbase::SPI_write_register (uint8_t command, uint8_t data )
{
	uint8_t retval = 1;				
	retval = SPI_transmit(command);
	if(!retval) retval = SPI_transmit(data);
	return retval;
}
uint8_t SPIbase::SPI_read_register (uint8_t command, uint8_t adress, uint8_t& receive)
{
	uint8_t retval = 1;	
	retval = SPI_transmit(command);
	if(!retval) retval = SPI_transmit(adress);
	if(!retval) retval = SPI_tranceive(0x00, receive);	
	return retval;	
}
uint8_t SPIbase::SPI_read_register (uint8_t command)
{
	uint8_t retval = 1;
	uint8_t receive = 0; 
	retval = SPI_transmit(command);
	if(!retval) retval = SPI_tranceive(0x00, receive);
	return retval;		
}
void SPIbase::enable_interrupt(void)
{
	cli();
	SPCR |= (1<<SPIE);	// Enable SPI Interrupt
	sei();
}
void SPIbase::disable_interrupt(void)
{	
	cli();
	SPCR &= ~(1<<SPIE);	//Disable interrupt
	sei();
}
SPI_TypeDef SPIbase::get_port()
{
	return _PORT;
}
void SPIbase::trigger_port(SPI_TypeDef PORT)
{
	for(uint8_t i = 0; i < SPIbase::ISR_LIST.get_size(); i++)
	{
		SPI_TypeDef port = SPIbase::ISR_LIST.get(i)->get_port();
		if(port == PORT)
		{
			SPIbase::ISR_LIST.get(i)->call_isr();
		}
	}	
}
void SPIbase::set_isr_cb(spi_isr_cb cb)
{
	_cb = cb;
	SPIbase::ISR_LIST.add(this);
}

void SPIbase::call_isr(void)
{
	_cb();
}	

/*SPIMaster::SPIMaster (GPIObase MOSI,GPIObase MISO,GPIObase SCK) : SPIbase(MOSI,MISO,SCK)
{
	
}*/
SPIMaster::SPIMaster (){}
//SPIMaster::SPIMaster (SPI_PORT SPI, Dout& CS, SPI_SPEED speed) : SPIbase(SPI, speed)
SPIMaster::SPIMaster (SPI_TypeDef SPI, Dout* CS, SPI_SPEED speed) : SPIbase(SPI, speed)
{
	//copy constructor but it is not used
	//non-static reference member can't use default assignment operator
	//_CS = Dout(CS);
	_CS = CS;	
}
//SPIMaster::SPIMaster (SPI_PORT SPI, Dout& CS, SPI_SPEED speed, spi_isr_cb cb) : SPIbase(SPI, speed ,cb)
SPIMaster::SPIMaster (SPI_TypeDef SPI, Dout* CS, SPI_SPEED speed, spi_isr_cb cb) : SPIbase(SPI, speed ,cb)
{
	//copy constructor but it is not used
	//non-static reference member can't use default assignment operator
	//_CS = Dout(CS);
	_CS = CS;	
}
SPIMaster::~SPIMaster (void)
{
	
}
void SPIMaster::SPI_init_master (void)
{
	if(_PORT == SP0)
	{
		this->SPI_CS_disable();
		//Disable SPIF - When a serial transfer is complete, the SPIF Flag is set
		SPSR &= ~(1<<SPIF);
		//Disable interrupt
		this->disable_interrupt();		
		if(_speed == SPI_LOW_SPEED )
		{
			// Enable SPI, Set as Master
			//Prescaler: Fosc/128-> 125kHz
			SPCR = (1<<SPE) | (1<<MSTR)| (1 << SPR1) | (1 << SPR0); //slow			
		}
		else if(_speed == SPI_HIGH_SPEED)
		{
			// Enable SPI, Set as Master
			SPCR = (1<<SPE) | (1<<MSTR);	
			//Prescaler: Fosc/2-> 8Mhz
			SPCR &= ~(1<<SPR0);
			SPCR &= ~(1<<SPR1);
			SPSR = (1<<SPI2X);			
		}
	}
}

void SPIMaster::SPI_CS_enable (void)
{
	//_SPI_PORT &= ~(1<<_CS._PIN);
	//_CS.write(GPIO_PIN_SET);
	_CS->write(GPIO_PIN_RESET);
}
void SPIMaster::SPI_CS_disable (void)
{
	//_SPI_PORT |= (1<<_CS._PIN);
	//_CS.write(GPIO_PIN_RESET);
	_CS->write(GPIO_PIN_SET);
}

SPISlave::SPISlave (){}

SPISlave::SPISlave (SPI_TypeDef SPI, SPI_SPEED speed) : SPIbase(SPI, speed)
{

}

SPISlave::SPISlave (SPI_TypeDef SPI, SPI_SPEED speed, spi_isr_cb cb) : SPIbase(SPI, speed, cb)
{

}
SPISlave::~SPISlave (void)
{
	
}
void SPISlave::SPI_init_slave (void)
{
	if(_PORT == SP0)
	{
		SPCR = (1<<SPE);				//Enable SPI
	}
}
