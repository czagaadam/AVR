/*
 * Timerbase.cpp
 *
 * Created: 2024. 06. 09. 20:50:24
 * Author : acer1
 */ 

#include <avr/io.h>
#include <avr/io.h>
#define F_CPU 16000000UL        //16Mhz
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <util/delay.h>
#include <stdbool.h>
#include "Timerbase.h"


TimerBase::TimerBase(){}
TimerBase::~TimerBase(){}

void TimerBase::trm1_1sec(void)
{
	// this code sets up a timer1 for 1s @ 16Mhz clock cycle
	// an interrupt is triggered each time the interval occurs.

	//TCCR1A	Timer/Counter Control Register 1 A
	//TCCR1B	Timer/Counter Control Register 1 B

	//output compare register
	OCR1A = 0x3D08;

	// Mode 4, CTC on OCR1A
	TCCR1B |= (1 << WGM12);

	//Set interrupt on compare match
	TIMSK1 |= (1 << OCIE1A);

	// set prescaler to 1024 and start the timer
	TCCR1B |= (1 << CS12) | (1 << CS10);

}

void TimerBase::TMR1_CTC_notworking(void)
{
	// this code sets up a timer1 for 1s @ 16Mhz clock cycle
	// an interrupt is triggered each time the interval occurs.
	//OCRn =  [ (clock_speed / Prescaler_value) * Desired_time_in_Seconds ] - 1
	//Now for the bad news OCRn has to be a whole number.
	// If you end up with a decimal number it means that your desired timer will not be exact.
	// The other thing is that your number has to be able to fit into the register.
	//So 255 for an 8bit timers and 65535 for the 16bit timer.
	//TCCR1A	Timer/Counter Control Register 1 A
	//TCCR1B	Timer/Counter Control Register 1 B
	//output compare register
	//(16000000/1024)-1
	OCR1A = 0x3D08;
	//ICR1 = 0xFFFF;
	//OCR1A = 0;
	// Mode 4, CTC on OCR1A
	TCCR1B |=  (1 << WGM12);
	//Set interrupt on compare match
	TIMSK1 |= (1 << OCIE1A);
	// set prescaler to 1024 and start the timer
	TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10);
	//SetBit(&DDRB,0);
}

void TimerBase::TMR1_CTC(void)
{
TCCR1B |= (1 << WGM12 ); // Configure timer 1 for CTC mode
TIMSK1 |= (1 << OCIE1A ) ; // Enable CTC interrupt
//sei () ; // Enable global interrupts
OCR1A = 15624; // Set CTC compare value to 1Hz at 1 MHz AVR clock , with a prescaler of 64
TCCR1B |= ((1 << CS10 ) | (1 << CS11 )) ; // Start timer at Fcpu /64
}

void TimerBase::TMR1_CTC_1ms(void)
{
	TCCR1B |= (1 << WGM12 ); // Configure timer 1 for CTC mode
	TIMSK1 |= (1 << OCIE1A ) ; // Enable CTC interrupt
	//sei () ; // Enable global interrupts
	OCR1A = 249; // Set CTC compare value to 1Hz at 1 MHz AVR clock , with a prescaler of 64
	TCCR1B |= ((1 << CS00 ) | (1 << CS01 )) ; // Start timer at Fcpu /64
}
