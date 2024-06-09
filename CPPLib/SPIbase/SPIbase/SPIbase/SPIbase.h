/*
 * SPIbase.h
 *
 * Created: 2024. 04. 26. 17:33:36
 *  Author: AMD_FX_X8
 */ 


#ifndef SPIBASE_H_
#define SPIBASE_H_

#include "GPIObase/GPIObase/GPIObase/GPIObase.h"
#include "GPIObase/GPIObase/GPIObase/ISRbase.h"




typedef struct SPI_PORT_0_TypeDef
{
	uint8_t MISO	= PORTB4;
	uint8_t MOSI	= PORTB3;
	uint8_t SCK		= PORTB5;
	volatile uint8_t* PORT	= &PORTB;
	volatile uint8_t* DIRREG_PORT = &DDRB;
}SPI_PORT_0;


typedef enum SPI_TypeDef
{
	SP0 = 0,
	SPI_PORT_1,
	SPI_PORT_2
}SPI_PORT;

typedef enum SPI_Speed_TypeDef
{
	SPI_LOW_SPEED = 0,
	SPI_HIGH_SPEED,
}SPI_SPEED;

//function pointer for callbacks
typedef void(*spi_isr_cb)(void);

class SPIbase
{
	public:
	uint8_t SPI_tranceive (uint8_t data, uint8_t& receive);
	uint8_t SPI_transmit (uint8_t data);
	uint8_t SPI_transmit (uint8_t data[], uint8_t nbyte);
	uint8_t SPI_receive (uint8_t data[], uint8_t *receive, uint8_t nbyte);
	uint8_t SPI_receive (uint8_t& receive);
	uint8_t SPI_write_register (uint8_t command, uint8_t adress, uint8_t data );
	uint8_t SPI_write_register (uint8_t command, uint8_t data );
	uint8_t SPI_read_register (uint8_t command, uint8_t adress, uint8_t& receive);
	uint8_t SPI_read_register (uint8_t command);
	spi_isr_cb _cb;	//function pointer
	
	void enable_interrupt(void);
	void disable_interrupt(void);
	static void trigger_port(SPI_PORT PORT);	//find SPI object in storage by SPI_TypeDef and call call_isr method of that object
	void set_isr_cb(spi_isr_cb cb);				//set function pointer
	void call_isr(void);							//call call back function pointed by function pointer	
	
	protected:
	SPIbase();
	SPIbase(SPI_TypeDef PORT, SPI_SPEED speed);
	SPIbase(SPI_TypeDef PORT, SPI_SPEED speed, spi_isr_cb cb);
	~SPIbase(void);
	//#todo: define MISO as Din 
	//#todo: define MOSI, SCK as Dout
	Dout _MOSI;
	Din _MISO;
	Dout _SCK;
	//volatile uint8_t* _SPI_PORT;
	//volatile uint8_t* _SPI_DIRREG_PORT;
	SPI_TypeDef _PORT;
	SPI_PORT_0 _SPI_PORT_0;
	static ISRbase<SPIbase> ISR_LIST;		//storage for SPI objects
	SPI_SPEED _speed;
	private:
		
};

class SPIMaster: public SPIbase
{
	public:
	SPIMaster ();
	//SPIMaster (SPI_PORT SPI, Dout& CS, SPI_SPEED speed);
	//SPIMaster (SPI_PORT SPI, Dout& CS, SPI_SPEED speed, spi_isr_cb cb);	
	SPIMaster (SPI_PORT SPI, Dout* CS, SPI_SPEED speed);
	SPIMaster (SPI_PORT SPI, Dout* CS, SPI_SPEED speed, spi_isr_cb cb);
	~SPIMaster (void);
	void SPI_init_master (void);
	//void SPI_init_master_interrupt (void);
	//void SPI_init_master_high_speed (void);
	void SPI_CS_enable (void);
	void SPI_CS_disable (void);

	//non-static reference member can't use default assignment operator
	//Dout& _CS;
	Dout* _CS;	
	
	protected:
	
	private:
	
};

class SPISlave: public SPIbase
{
	public:
	SPISlave ();
	SPISlave (SPI_PORT SPI, SPI_SPEED speed);
	SPISlave (SPI_PORT SPI, SPI_SPEED speed, spi_isr_cb cb);	
	~SPISlave (void);
	void SPI_init_slave (void);	
	protected:
		
	private:
		
};


#endif /* SPIBASE_H_ */