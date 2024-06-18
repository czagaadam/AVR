/*
 * CPP_SPI_test.cpp
 *
 * Created: 2024. 05. 09. 21:19:57
 * Author : acer1
 */ 

#include <util/delay.h>
#include "GPIObase/GPIObase/GPIObase/GPIObase.h"
#include "UARTbase/UARTbase/UARTbase.h"
#include "SPIbase/SPIbase/SPIbase/SPIbase.h"
#include "SPI_MCP3201_ADC/MCP3201/MCP3201.h"

SPIMaster SPI;
UARTbase UART;
MCP3201_ADC MCP3201;
Dout CS;
Dout LED7;

ISR(USART_RX_vect)
{
	cli();
	UARTbase::trigger_port(UART0);
	sei();
}

ISR(SPI_STC_vect)
{
	cli();
	SPIbase::trigger_port(SP0);
	sei();
}

void UART_callback(void)
{
	unsigned char dummy;
	dummy = UDR0;                     // dump the rcvr buffer
	while (!( UCSR0A & (1<<UDRE0)));  // wait until the register is free
}

void SPI_callback(void)
{
	UART.send_string("CALLBACK");	
}

int main(void)
{
	cli();
	UART = UARTbase(UART0,UART_callback);
	
	//#todo: check constructor, without write(GPIO_PIN_SET) it is still off
	CS = Dout(&PORTB,PORTB2,GPIO_PIN_RESET);
	CS.write(GPIO_PIN_RESET);//????????????????????????????????????,
	
	LED7 = Dout(&PORTD,PORTD7,GPIO_PIN_RESET);
	LED7.write(GPIO_PIN_RESET);
	
	SPI = SPIMaster(SP0, &CS, SPI_LOW_SPEED/*, SPI_callback*/);
	//SPI.disable_interrupt();	
	SPI.SPI_init_master();
	
	UART.init();
	UART.enable_interrupt();
	UART.send_string("START");
	//SPI.enable_interrupt();
	
	MCP3201 = MCP3201_ADC(&SPI, &CS);	
	
	sei();
	_delay_ms(250);
	while (1)
	{
		_delay_ms(250);_delay_ms(250);
		LED7.write(GPIO_PIN_SET);
		char buffer[50] = "";
		MCP3201.read(buffer);
		UART.send_string(buffer);
		_delay_ms(250);_delay_ms(250);
		LED7.write(GPIO_PIN_RESET);
	}
}

