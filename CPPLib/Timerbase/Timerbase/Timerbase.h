/*
 * Timerbase.h
 *
 * Created: 2024. 06. 09. 20:50:53
 *  Author: acer1
 */ 


#ifndef TIMERBASE_H_
#define TIMERBASE_H_

#include <avr/io.h>
#include <avr/io.h>
#define F_CPU 16000000UL        //16Mhz
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <util/delay.h>
#include <stdbool.h>

class TimerBase
{
	public:
	TimerBase();
	~TimerBase();
	void trm1_1sec(void);
	void TMR1_CTC(void);
	void TMR1_CTC_notworking(void);
	void TMR1_CTC_1ms(void);
};


#endif /* TIMERBASE_H_ */