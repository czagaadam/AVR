/*
 * SPIbase.cpp
 *
 * Created: 2024. 04. 26. 17:33:20
 *  Author: AMD_FX_X8
 */ 
#include <avr/io.h>
#include "SPIbase.h"

SPIbase::SPIbase(GPIObase CS,GPIObase MOSI,GPIObase MISO,GPIObase SCK)
{
	this->_CS = CS;
	this->_MISO = MISO;
	this->_MOSI = MOSI;
	this->_SCK = SCK;
}
SPIbase::~SPIbase(void)
{
	
}
uint8_t SPIbase::SPI_CS_enable (void){
	SPI_PORT &= ~(1<<CS);
}
uint8_t SPIbase::SPI_CS_disable (void){}
uint8_t SPIbase::SPI_tranceive (uint8_t data){}
uint8_t SPIbase::SPI_multi_tranceive (uint8_t data[], uint8_t nbyte){}
uint8_t SPIbase::SPI_multi_receive (uint8_t data[], uint8_t* receive, uint8_t nbyte){}
uint8_t SPIbase::SPI_receive (void){}
uint8_t SPIbase::SPI_write_register (uint8_t command, uint8_t adress, uint8_t data ){}
uint8_t SPIbase::SPI_write_register_by_instruction (uint8_t command, uint8_t data ){}
uint8_t SPIbase::SPI_read_register (uint8_t command, uint8_t adress){}
uint8_t SPIbase::SPI_read_register_by_instruction (uint8_t command){}
	

