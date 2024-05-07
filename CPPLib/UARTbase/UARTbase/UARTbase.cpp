#include <avr/pgmspace.h>
#include <avr/io.h>
#define F_CPU 16000000UL        //16Mhz
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>       // needed for uint8_t
#include <stdio.h>
#include <string.h>
#define USART_BAUDRATE 9600
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void USART0Init(void)
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

void USART0SendByte(uint8_t u8Data)
{
	//wait while previous byte is completed
	//checking transmission buffer empty flag UDRE0 in UCSR0A register
	while(!(UCSR0A&(1<<UDRE0))){};
	//sending data
	//UDR0 in USART0 module which is used to send and receive data
	UDR0 = u8Data;
}

void USART0SendString(char* sData)
{
	while (*sData)
	{
		USART0SendByte(*sData++);
	}
}

/** Sending integer 
*
*
* @param name component of interest
* @param verbosity desired trace verbosity
*/
void USART0SendInt(uint16_t iData)
{
	char buffer[10];
	itoa(iData,buffer,10);
	USART0SendString("USART0SendInt2: ");
	USART0SendString(buffer);
	USART0SendString("\r\n");	
}

void USART0SendInt(uint16_t iData, char* &buffer)
{
	itoa(iData,buffer,10);
	USART0SendString("USART0SendInt3: ");
	USART0SendString(buffer);
	USART0SendString("\r\n");
}

void USART0SendIntWithText(uint16_t iData, char Text[])
{
	char buffer[10];
	itoa(iData,buffer,10);
	USART0SendString(Text);
	USART0SendString(buffer);
	USART0SendString("\r\n");
}

void USART0SendIntWithText(uint16_t iData, char* &buffer, char Text[])
{
	itoa(iData,buffer,10);
	USART0SendString(Text);
	USART0SendString(buffer);
	USART0SendString("\r\n");
}

uint8_t USART0ReceiveByte()
{
	// Wait for byte to be received
	while(!(UCSR0A&(1<<RXC0))){};
	// Return received data
	return UDR0;
}

void USART0_Interrupt_Init(void)
{
	cli();
	UCSR0B = ((1<<RXEN0)|(1<<TXEN0)|(1 << RXCIE0));       // Enable receiver and transmitter and Rx interrupt
	sei();
}





