/*
 * MCP3201.cpp
 *
 *  Author: Adam Czaga czagaadam@gmail.com
 *
 *  MCP3201 ADC class
 *
 */ 

#include <avr/io.h>

#include "MCP3201.h"

MCP3201_ADC::MCP3201_ADC(){}
MCP3201_ADC::MCP3201_ADC(SPIMaster* PORT, Dout* CS)
{
	_PORT = PORT;
	_CS = CS;
}
	
MCP3201_ADC::~MCP3201_ADC(){}
	
uint8_t MCP3201_ADC::read_adc(uint16_t& ADCVal)
{
	uint8_t MSB,LSB;
	_PORT->SPI_CS_enable();
	_PORT->SPI_receive(MSB);	//2bytes dummy write operation for generating 16 clock cycles
	_PORT->SPI_receive(LSB);		//and reading the spi buffer
	_PORT->SPI_CS_disable();
	MSB &= ~(0xE0);			//same as previous 2 lines
	ADCVal = MSB;			//Copy the 8bit MSB to 16bit variable 0000000000010101 -> 0001010100000000
	ADCVal = ADCVal << 8;	//Right shift the 8bit MSB inside the 16bit variable
	ADCVal |= LSB;
	ADCVal = ADCVal >>4;	//cut LSB first 4 bits
	//_PORT->SPI_CS_disable();
	return 0;
}
	
float MCP3201_ADC::convert_to_float(uint16_t ADCVal)
{
	float ADC_float;
	ADC_float = ADCVal / 100.0;			//read ADC
	return ADC_float;
}
	
void MCP3201_ADC::float_to_string(float ADC_float, char* buffer)
{
	char floatbuffer[10] ="";
	dtostrf( ADC_float, 2, 2, floatbuffer );		//float to string
	sprintf (buffer, "ADC: %4sV", floatbuffer);
}
	
void MCP3201_ADC::read(char* buffer)
{
	uint16_t ADCVal;
	if (!read_adc(ADCVal))
	{	
		float_to_string(convert_to_float(ADCVal),buffer);
	}else
	{
		//
	}

}

