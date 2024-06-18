/*
 * UARTbase.cpp
 *
 *  Author: Adam Czaga czagaadam@gmail.com
 *
 *  UART base class
 *
 */ 

#include <avr/pgmspace.h>
#include <avr/io.h>
#define F_CPU 16000000UL    //16Mhz
#include <avr/interrupt.h>	//cli sei
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "UARTbase.h"
#include "GPIObase/GPIObase/GPIObase/ISRbase.h"
#define USART_BAUDRATE 9600
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)
#define UNOBOARD 1
#define MEGABOARD 0

ISRbase<UARTbase> UARTbase::ISR_LIST;

UARTbase::UARTbase(){}

UARTbase::UARTbase(UART_TypeDef PORT)
{
	_PORT = PORT;
}

UARTbase::UARTbase(UART_TypeDef PORT, uart_isr_cb cb)
{
	_PORT = PORT;
	//UARTbase::enable_interrupt();
	UARTbase::set_isr_cb(cb);
}

UARTbase::~UARTbase(){
	//#todo:
}

void UARTbase::init(void)
{
	#ifdef UNOBOARD	
		if(_PORT == UART0)
		{
			// Set baud rate
			//high 8 bit
			UBRR0H = (uint8_t)(UBRR_VALUE>>8);
			//low 8 bit
			UBRR0L = (uint8_t)UBRR_VALUE;
			//or UBRR0= UBRR_VALUE;
			// Set frame format to 8 data bits, no parity, 1 stop bit
			//UCSR0C |= (1<<UCSZ1)|(1<<UCSZ0);
			//Default frame format is 8 data bits, no parity, 1 stop bit
			//enable transmission and reception
			UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
		}
	#endif
	//#todo:
}

void UARTbase::send_byte(uint8_t u8Data)
{
	#ifdef UNOBOARD
		if(_PORT == UART0)
		{
			//wait while previous byte is completed
			//checking transmission buffer empty flag UDRE0 in UCSR0A register
			while(!(UCSR0A&(1<<UDRE0))){};
			//sending data
			//UDR0 in USART0 module which is used to send and receive data
			UDR0 = u8Data;
		}
	#endif
	//#todo:
}

void UARTbase::new_line()
{
	UARTbase::send_byte(13); // \r is the Carriage Return character 0x0D (decimal 13).
	UARTbase::send_byte(10); // \n is the Newline character 0x0A (decimal 10)
}

void UARTbase::send_string(char* sData)
{
	while (*sData)
	{
		UARTbase::send_byte(*sData++);
	}
	UARTbase::new_line();
}

/** Sending integer via COM port
*
* @param iData input number
*/
void UARTbase::send_int(uint16_t iData)
{
	char buffer[10];
	itoa(iData,buffer,10);
	UARTbase::send_string(buffer);
	memset(buffer, 0, sizeof buffer);	
}
/** Sending integer via COM port
*
* @param iData input number
* @param buffer reference for future use of transfered data
*/
void UARTbase::send_int(uint16_t iData, char* &buffer)
{
	itoa(iData,buffer,10);
	UARTbase::send_string(buffer);
}
void UARTbase::send_int_with_text(uint16_t iData, char Text[])
{
	char buffer[10];
	itoa(iData,buffer,10);
	send_string(Text);
	send_string(buffer);
	send_string("\r\n");
	memset(buffer, 0, sizeof buffer);
}
void UARTbase::send_int_with_text(uint16_t iData, char* &buffer, char Text[])
{
	itoa(iData,buffer,10);
	send_string(Text);
	send_string(buffer);
	send_string("\r\n");
}


void UARTbase::send_float(float data)
{
	char buffer[10] = "";
	char floatbuffer[10] ="";
	dtostrf( data, 2, 2, floatbuffer );				//float to string
	//#todo: add text and length 
	sprintf (buffer, "ADC: %4sV\r\n", floatbuffer);
	UARTbase::send_string(buffer);
	memset(floatbuffer, 0, sizeof floatbuffer);
	memset(buffer, 0, sizeof buffer);
}


uint8_t UARTbase::receive_byte()
{
	#ifdef UNOBOARD
		if(_PORT == UART0)
		{
		// Wait for byte to be received
		while(!(UCSR0A&(1<<RXC0))){};
		// Return received data
		return UDR0;
		}
		else
		{
		return 0;
		}	
	#endif
	//#todo:
}

void UARTbase::enable_interrupt(void)
{
	#ifdef UNOBOARD
		if(_PORT == UART0)
		{
			cli();
			//UCSR0B = ((1<<RXEN0)|(1<<TXEN0)|(1 << RXCIE0));       // Enable receiver and transmitter and Rx interrupt
			UCSR0B |= ((1 << RXCIE0));       // Rx interrupt
			sei();
		}
	#endif
	//#todo:
}

UART_TypeDef UARTbase::get_port()
{
	return _PORT;
}

void UARTbase::trigger_port(UART_TypeDef PORT)
{
	for(uint8_t i = 0; i < UARTbase::ISR_LIST.get_size(); i++)
	{
		UART_TypeDef port = UARTbase::ISR_LIST.get(i)->get_port();
		if(port == PORT)
		{
			UARTbase::ISR_LIST.get(i)->call_isr();
		}
	}	
}

void UARTbase::set_isr_cb(uart_isr_cb cb)
{
	_cb = cb;	//store (callback) function pointer
	UARTbase::ISR_LIST.add(this);
}
void UARTbase::call_isr(void)
{
	_cb();
}




