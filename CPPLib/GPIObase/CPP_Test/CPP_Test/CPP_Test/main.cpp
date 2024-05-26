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

Dout LED7;
Dout LED5;
Din Button1;
Din Button2;
UARTbase UART;

ISR(USART_RX_vect)
{
	cli();
	UART.trigger_port(UART0);
	sei();
}

ISR(PCINT0_vect)
{ 
	cli();
	if (Button1.read() == GPIO_PIN_RESET)
	{
		//Din::trigger_pin(PINB0);	//trigger callback
		Din::trigger_pin(Button1.get_pin());
	}
	if (Button2.read() == GPIO_PIN_RESET)
	{		
		//Din::trigger_pin(PINB1);	//trigger callback
		Din::trigger_pin(Button2.get_pin());
	}	
	sei();
}
void Button1_callback(void)
{
	UART.send_string("Button1_callback");
	//LED7.write(GPIO_PIN_SET);
}

void Button2_callback(void)
{
	UART.send_string("Button2_callback");
	//LED7.write(GPIO_PIN_RESET);
}

void UART_callback(void)
{
	unsigned char dummy;
	dummy = UDR0;                     // dump the rcvr buffer
	while (!( UCSR0A & (1<<UDRE0)));  // wait until the register is free
	LED7.write(GPIO_PIN_SET);
	_delay_ms(250);
	_delay_ms(250);
	LED7.write(GPIO_PIN_RESET);
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


int main(void)
{
	cli();
	//#todo: static function
	PCICR |= 0b00000001; // Enables Ports B Pin Change Interrupts
	PCMSK0 |= 0b00000011; // PCINT0 PCINT1
	//PCMSK2 |= 0b10000000; // PCINT23	
	UART = UARTbase(UART0,UART_callback);		
	UART.init();
	UART.enable_interrupt();
	UART.send_string("START");
	sei();
	//#todo: check constructor, without write(GPIO_PIN_SET) it is still off
	LED7 = Dout(&PORTD,PORTD7,GPIO_PIN_SET);
	LED7.write(GPIO_PIN_SET);//????????????????????????????????????,
	//LED5 = Dout(&PORTB,PORTB5);
	Button1 = Din(&PORTB,PORTB0,GPIO_PULL_UP,Button1_callback);
	Button2 = Din(&PORTB,PORTB1,GPIO_PULL_UP,Button2_callback);
	

	LED7.write(GPIO_PIN_SET);
	_delay_ms(250);
	_delay_ms(250);
	_delay_ms(250);
	_delay_ms(250);
	_delay_ms(250);
	_delay_ms(250);
	_delay_ms(250);
	_delay_ms(250);
		

    while (1) 
    {
		_delay_ms(1);
    }
}




