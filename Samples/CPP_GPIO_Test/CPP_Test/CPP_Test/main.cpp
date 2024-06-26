/*
 * CPP_Test.cpp
 *
 * Created: 2024. 04. 28. 23:04:47
 * Author : acer1
 */ 

#include <avr/pgmspace.h>
#include <avr/io.h>
#define F_CPU 16000000UL        //16Mhz
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>       // needed for uint8_t
#include <stdio.h>
#include <string.h>
#include "GPIObase/GPIObase/GPIObase/GPIObase.h"
#include "UARTbase/UARTbase/UARTbase.h"

Dout LED_B2;
Dout LED_D7;
Din Button1;
Din Button2;
UARTbase UART;

ISR(USART_RX_vect)
{
	cli();
	UARTbase::trigger_port(UART0);
	sei();
}

ISR(PCINT0_vect)
{ 
	cli();
	if (Button1.read() == GPIO_PIN_RESET)
	{
		Din::trigger_pin(Button1.get_pin());	//trigger callback
	}
	if (Button2.read() == GPIO_PIN_RESET)
	{		
		Din::trigger_pin(Button2.get_pin());	//trigger callback
	}	
	sei();
}
void Button1_callback(void)
{
	UART.send_string("Button1_callback");
	//LED_B2.write(GPIO_PIN_SET);
}

void Button2_callback(void)
{
	UART.send_string("Button2_callback");
	//LED_B2.write(GPIO_PIN_RESET);
}

void UART_callback(void)
{
	char dummy;
	dummy = UDR0;                     // dump the rcvr buffer
	while (!( UCSR0A & (1<<UDRE0)));  // wait until the register is free
	UART.send_string(&dummy);
}

int main(void)
{
	cli();
	UART = UARTbase(UART0,UART_callback);		
	UART.init();
	UART.enable_interrupt();
	UART.send_string("START");
	
	//#todo: check constructor, without write(GPIO_PIN_SET) it is still off
	LED_B2 = Dout(&PORTB,PORTB2,GPIO_PIN_RESET);
	LED_B2.write(GPIO_PIN_RESET);//????????????????????????????????????,
	LED_D7 = Dout(&PORTD,PORTD7,GPIO_PIN_RESET);
	LED_D7.write(GPIO_PIN_RESET);
	Button1 = Din(&PORTB,PORTB0,GPIO_PULL_UP,Button1_callback);
	Button2 = Din(&PORTB,PORTB1,GPIO_PULL_UP,Button2_callback);
	Button1.enable_interrupt();
	Button2.enable_interrupt();
	sei();
	
	LED_B2.write(GPIO_PIN_SET);
	_delay_ms(100);

    while (1) 
    {
		LED_B2.write(GPIO_PIN_RESET);
		LED_D7.write(GPIO_PIN_RESET);
		//PORTD |= (1 << PINB0);
		_delay_ms(250);
		LED_B2.write(GPIO_PIN_SET);
		LED_D7.write(GPIO_PIN_SET);
		//PORTD &= ~(1 << PINB0);
		_delay_ms(250);
    }
}


typedef void (*func_t)(void);

void callFunctionAt (func_t address)
{
	address();
}

//get callback addresses for testing
//0x1db
//0x221
/*char buffer[10];
sprintf(buffer, "%p",  (void*)Button1._cb);
USART0SendString(buffer);
char buffer2[10];
sprintf(buffer2, "%p",  (void*)Button2._cb);
USART0SendString(buffer2);*/

//direct callback by address for testing
void call_0x221()
{
	callFunctionAt ((func_t) 0x221);
}
void call_0x23e()
{
	callFunctionAt ((func_t) 0x23e);
}

//call_0x221();
//call_0x23e();

