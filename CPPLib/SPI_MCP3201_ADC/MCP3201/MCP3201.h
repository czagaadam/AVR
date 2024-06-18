/*
 * MCP3201.h
 *
 *  Author: Adam Czaga czagaadam@gmail.com
 *
 *  MCP3201 ADC class
 *
 */ 


#ifndef MCP3201_H_
#define MCP3201_H_

#include <avr/pgmspace.h>
#include <avr/io.h>
#define F_CPU 16000000UL        //16Mhz
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "GPIObase/GPIObase/GPIObase/GPIObase.h"
#include "SPIbase/SPIbase/SPIbase/SPIbase.h"

class MCP3201_ADC
{
	public:
	MCP3201_ADC();
	~MCP3201_ADC();
	MCP3201_ADC(SPIMaster* PORT, Dout* CS);
	
	
	
	uint8_t read_adc(uint16_t& ADCVal);
	
	float convert_to_float(uint16_t ADCVal);
	
	void float_to_string(float ADC_float, char* buffer);
	
	void read(char* buffer);
	protected:
	SPIMaster*_PORT;
	Dout* _CS;
};



#endif /* MCP3201_H_ */