/*
 * SPIbase.h
 *
 * Created: 2024. 04. 26. 17:33:36
 *  Author: AMD_FX_X8
 */ 


#ifndef SPIBASE_H_
#define SPIBASE_H_

#include "GPIObase/GPIObase/GPIObase/GPIObase.h"


/*typedef struct SPI0_PORT
{
	uint8 _MISO	= PORTB4;
	_MOSI	= PORTB3;
	_SCK	= PORTB5;
	_SPI_PORT = PORTB;
	_SPI_DIRREG_PORT = DDRB
}SPI0_PORT;*/

typedef struct SPI0
{
	uint8_t MISO	= PORTB4;
	uint8_t MOSI	= PORTB3;
	uint8_t SCK		= PORTB5;
	volatile uint8_t* PORT	= &PORTB;
	volatile uint8_t* DIRREG_PORT = &DDRB;
}SPI0;

typedef enum SPI_PORT
{
	SPI_PORT_0 = 0,
	SPI_PORT_1,
	SPI_PORT_2
}SPI_PORT;

class SPIbase
{
	public:
	uint8_t SPI_tranceive (uint8_t data);
	uint8_t SPI_multi_tranceive (uint8_t data[], uint8_t nbyte);
	uint8_t SPI_multi_receive (uint8_t data[], uint8_t* receive, uint8_t nbyte);
	uint8_t SPI_receive (void);
	uint8_t SPI_write_register (uint8_t command, uint8_t adress, uint8_t data );
	uint8_t SPI_write_register_by_instruction (uint8_t command, uint8_t data );
	uint8_t SPI_read_register (uint8_t command, uint8_t adress);
	uint8_t SPI_read_register_by_instruction (uint8_t command);
	protected:
	SPIbase(SPI_PORT PORT);
	~SPIbase(void);
	//#todo: define MISO as Din 
	//#todo: define MOSI, SCK as Dout
	Dout _MOSI;
	Din _MISO;
	Dout _SCK;
	//volatile uint8_t* _SPI_PORT;
	//volatile uint8_t* _SPI_DIRREG_PORT;
	SPI_PORT _PORT;
	SPI0 _SPI0;
	private:
		
};

class SPIMaster: public SPIbase
{
	public:
	SPIMaster (SPI_PORT SPI);
	~SPIMaster (void);
	void SPI_init_master (void);
	void SPI_init_master_interrupt (void);
	void SPI_init_master_high_speed (void);
	protected:
	
	private:
	
};

class SPISlave: public SPIbase
{
	public:
	SPISlave (SPI_PORT SPI, Dout& CS);
	~SPISlave (void);
	void SPI_init_slave (void);
	void SPI_CS_enable (void);
	void SPI_CS_disable (void);	
	protected:
	Dout& _CS;	
	private:
		
};


#endif /* SPIBASE_H_ */