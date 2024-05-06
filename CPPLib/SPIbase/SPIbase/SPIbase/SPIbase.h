/*
 * SPIbase.h
 *
 * Created: 2024. 04. 26. 17:33:36
 *  Author: AMD_FX_X8
 */ 


#ifndef SPIBASE_H_
#define SPIBASE_H_

#include "GPIObase.h"

class SPIbase
{
	public:
	void SPI_CS_enable (uint8_t SPI_PORT);
	uint8_t SPI_CS_enable (void);
	uint8_t SPI_CS_disable (void);
	uint8_t SPI_tranceive (uint8_t data);
	uint8_t SPI_multi_tranceive (uint8_t data[], uint8_t nbyte);
	uint8_t SPI_multi_receive (uint8_t data[], uint8_t* receive, uint8_t nbyte);
	uint8_t SPI_receive (void);
	uint8_t SPI_write_register (uint8_t command, uint8_t adress, uint8_t data );
	uint8_t SPI_write_register_by_instruction (uint8_t command, uint8_t data );
	uint8_t SPI_read_register (uint8_t command, uint8_t adress);
	uint8_t SPI_read_register_by_instruction (uint8_t command);
	protected:
	SPIbase (GPIObase CS,GPIObase MOSI,GPIObase MISO,GPIObase SCK);
	~SPIbase (void);
	
	GPIObase _CS;
	GPIObase _MOSI;
	GPIObase _MISO;
	GPIObase _SCK;
	private:
		
};

class SPIMaster() : public SPIbase()
{
	public:
	SPIMaster (GPIObase CS,GPIObase MOSI,GPIObase MISO,GPIObase SCK);
	~SPIMaster (void);
	uint8_t SPI_init_master (void);
	uint8_t SPI_init_master_interrupt (void);
	uint8_t SPI_init_master_high_speed (void);
	protected:
	
	private:
	
};

class SPISlave() : public SPI()
{
	public:
	SPISlave (GPIObase CS,GPIObase MOSI,GPIObase MISO,GPIObase SCK);
	~SPISlave (void);
	uint8_t SPI_init_slave (void);
	protected:
	
	private:
	
};


#endif /* SPI_H_ */



#endif /* SPIBASE_H_ */