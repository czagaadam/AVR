/*
 * SPIbase.cpp
 *
 * Created: 2024. 04. 26. 17:33:20
 *  Author: AMD_FX_X8
 */ 
#include <avr/io.h>
#include "SPIbase.h"

//GPIObase CS,GPIObase MOSI,GPIObase MISO,GPIObase SCK
/*
SPIbase::SPIbase(GPIObase MOSI,GPIObase MISO,GPIObase SCK)
{
	_MISO	= MISO;
	_MOSI	= MOSI;
	_SCK	= SCK;
	_SPI_PORT = MOSI._PORT;
	_SPI_DIRREG_PORT = MOSI._DDR;
}*/
SPIbase::SPIbase(SPI_PORT SPI)
{
	if(SPI == SPI_PORT_0)
	{	
		/*#define CS PORTB2
		#define MOSI PORTB3
		#define MISO PORTB4
		#define SCK PORTB5	*/	
		//this->_CS = CS;
		/*_MISO.set_pin(_SPI0.MISO);
		_MOSI.set_pin(_SPI0.MOSI);
		_SCK.set_pin(_SPI0.SCK);*/
		_MOSI = Dout(_SPI0.PORT, _SPI0.MOSI, GPIO_PIN_RESET);
		_MISO = Din(_SPI0.PORT, _SPI0.MISO);
		_SCK  = Dout(_SPI0.PORT, _SPI0.SCK, GPIO_PIN_RESET);
		//_SPI_PORT = _SPI0.PORT;
		//_SPI_DIRREG_PORT = _SPI0.DIRREG_PORT;
		_PORT = SPI;
	}
	//#todo:
}
SPIbase::~SPIbase(void)
{
	
}

//#todo: implement timeout!!!!!!!!!!!!!!!!
unsigned  SPIbase::SPI_tranceive (uint8_t data)
{	// Load data into the buffer
	SPDR = data;
	
	//Wait until transmission complete
	while(!(SPSR & (1<<SPIF) ));
	
	// Return received data
	return(SPDR);
}

uint8_t SPIbase::SPI_multi_tranceive (uint8_t data[], uint8_t nbyte)
{
	uint8_t retVal = 0x00;
	for (uint8_t i=0; i<nbyte ;i++ ) {
		retVal = SPI_tranceive(data[i]);
	}
	return(retVal);
}

void SPIbase::SPI_multi_receive (uint8_t data[], uint8_t* receive, uint8_t nbyte)
{
	if(nbyte<100)
	{
		uint8_t retVal[100];
		for (uint8_t i=0; i<nbyte;i++)
		{
			retVal[i] = SPI_tranceive(data[i]);
		}
		memcpy(receive, retVal, nbyte);
	}
	else
	{
		memcpy(receive, 0x00, 1);
	}
}
uint8_t SPIbase::SPI_receive (void){}
uint8_t SPIbase::SPI_write_register (uint8_t command, uint8_t adress, uint8_t data ){}
uint8_t SPIbase::SPI_write_register_by_instruction (uint8_t command, uint8_t data ){}
uint8_t SPIbase::SPI_read_register (uint8_t command, uint8_t adress){}
uint8_t SPIbase::SPI_read_register_by_instruction (uint8_t command){}
	
/*SPIMaster::SPIMaster (GPIObase MOSI,GPIObase MISO,GPIObase SCK) : SPIbase(MOSI,MISO,SCK)
{
	
}*/
SPIMaster::SPIMaster (SPI_PORT SPI) : SPIbase(SPI)
{
	
}
SPIMaster::~SPIMaster (void)
{
	
}
void SPIMaster::SPI_init_master (void)
{
	if(_PORT == SPI_PORT_0)
	{	
		// Set MOSI, SCK Output
		//*_SPI_DIRREG_PORT = (1<<_SCK.get_pin())|(1<<_MOSI.get_pin())/*|(1<<_CS)*/;

		//Disable SPIF - When a serial transfer is complete, the SPIF Flag is set
		SPSR &= ~(1<<SPIF);
		
		//Disable interrupt
		SPCR &= ~(1<<SPIE);
		
		// Enable SPI, Set as Master
		//Prescaler: Fosc/128-> 125kHz
		SPCR = (1<<SPE) | (1<<MSTR)| (1 << SPR1) | (1 << SPR0); //slow
	}
}
void SPIMaster::SPI_init_master_interrupt (void)
{
	if(_PORT == SPI_PORT_0)
	{
		// Set MOSI, SCK as Output
		//*_SPI_DIRREG_PORT = (1<<_SCK.get_pin())|(1<<_MOSI.get_pin())/*|(1<<CS)*/;
		
		// Enable SPI, Set as Master
		// Prescaler: Fosc/128-> 125kHz, Enable Interrupts
		//The MOSI, SCK pins are as per ATMega8
		SPCR=(1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<SPIE);
		
		// Enable Global Interrupts
		//sei();
	}
}
void SPIMaster::SPI_init_master_high_speed (void)
{
	if(_PORT == SPI_PORT_0)
	{
		// Set MOSI, SCK as Output
		//*_SPI_DIRREG_PORT = (1<<_SCK.get_pin())|(1<<_MOSI.get_pin())/*|(1<<CS)*/;
		//SPI_CS_disable();	//cs high
		//Disable SPIF - When a serial transfer is complete, the SPIF Flag is set
		SPSR &= ~(1<<SPIF);
		
		//Disable interrupt
		SPCR &= ~(1<<SPIE);
		
		// Enable SPI, Set as Master
		SPCR = (1<<SPE) | (1<<MSTR);
		
		//Prescaler: Fosc/2-> 8Mhz
		SPCR &= ~(1<<SPR0);
		SPCR &= ~(1<<SPR1);
		SPSR = (1<<SPI2X);
	}
}



SPISlave::SPISlave (SPI_PORT SPI, Dout& CS) : SPIbase(SPI), _CS(CS)
{
	//_CS = &CS;
}
SPISlave::~SPISlave (void)
{
	
}
void SPISlave::SPI_init_slave (void)
{
	if(_PORT == SPI_PORT_0)
	{
		//SPI_PORT = (1<<_MISO._PIN);	//MISO as OUTPUT
		SPCR = (1<<SPE);				//Enable SPI
	}
}
void SPISlave::SPI_CS_enable (void)
{
	//_SPI_PORT &= ~(1<<_CS._PIN);
	_CS.write(GPIO_PIN_SET);
}
void SPISlave::SPI_CS_disable (void)
{
	//_SPI_PORT |= (1<<_CS._PIN);
	_CS.write(GPIO_PIN_RESET);
}