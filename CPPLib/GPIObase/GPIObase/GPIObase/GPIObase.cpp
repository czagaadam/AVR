/*
 * GPIObase.cpp
 *
 *  Author: Adam Czaga czagaadam@gmail.com
 *
 *  GPIO parent class and Din,Dout inherited classes
 *
 */ 

#include "GPIObase.h"
#include "ISRbase.h"

ISRbase<Din> Din::ISR_LIST;

GPIObase::GPIObase(){}
GPIObase::GPIObase(volatile uint8_t* PORT, uint8_t PIN)
{
	_PIN = PIN;
	_PORT = PORT;
	GPIObase::init();
}
GPIObase::~GPIObase(){
	//#todo:
}	
void GPIObase::init()
{
	if(_PORT == &PORTB)
	{
		_DDR = &DDRB;
		_PINgroup = &PINB;
	}
	if(_PORT == &PORTC)
	{
		_DDR = &DDRC;
		_PINgroup = &PINC;
	}
	if(_PORT == &PORTD)
	{
		_DDR = &DDRD;
		_PINgroup = &PIND;
	}
	/*_DDR = &DDRB;
	_PINgroup = &PINB;*/
}
GPIO_PinState GPIObase::read(void)	
{
	//#todo: check and implement for other ports
	//if(!(PINB & (1 << PORTB0))) return 0; else return 1;
	
	//if ( ( ( (*_PINgroup) & ( 1 << _PIN ) ) >> _PIN ) == 0 ) return 0;  else return 1;
	//if ( ( ( (*_PINgroup) & ( 1 << _PIN ) ) >> _PIN ) == 0 ) return 0;  else return 1;
	
	if(*_PORT == PORTB)
	{
		if ( ( ( (PINB) & ( 1 << _PIN ) ) >> _PIN ) == 0 ) return GPIO_PIN_RESET;  else return GPIO_PIN_SET;
	}
	//#todo: else
	/*if(*_PORT == PORTC)
	{
		DDRC &= ~(1 << _PIN);
	}
	if(*_PORT == PORTD)
	{
		DDRD &= ~(1 << _PIN);
	}
	_PIN & (1 << _PIN)
	if (((*_PORT) & (1 << _PIN) >> _PIN) == 0){ return 0;}
	else {return 1;}*/
}	
void GPIObase::pull_up(void)
{
	*_PORT |= (1 << _PIN);
}
void GPIObase::pull_down(void)
{
	*_PORT &= ~(1 << _PIN);
}
uint8_t GPIObase::get_pin(void)
{
	return _PIN;
}

Din::Din(){}
Din::~Din(){Din::ISR_LIST.remove(this);}
Din::Din(volatile uint8_t* PORT, uint8_t PIN) : GPIObase(PORT, PIN)
{
	*_DDR &= ~(1 << _PIN);
	//#todo: defult PUPD???
}
Din::Din(volatile uint8_t* PORT, uint8_t PIN, GPIO_Pull pull) : GPIObase(PORT, PIN)
{
	*_DDR &= ~(1 << _PIN);
	if (pull == GPIO_PULL_UP)	GPIObase::pull_up();
	else GPIObase::pull_down();	
}
Din::Din(volatile uint8_t* PORT, uint8_t PIN, GPIO_Pull pull, gpio_isr_cb cb) : GPIObase(PORT, PIN)
{
	*_DDR &= ~(1 << _PIN);
	if (pull == GPIO_PULL_UP)	GPIObase::pull_up();
	else GPIObase::pull_down();
	Din::set_isr_cb(cb);
}

void Din::set_isr_cb(gpio_isr_cb cb)
{
	_cb = cb;	//store (callback) function pointer
	Din::ISR_LIST.add(this);	
}
void Din::call_isr(void)
{
	_cb();
}
void Din::trigger_pin(uint8_t gpio_pin)
{
	for(uint8_t i = 0; i < Din::ISR_LIST.get_size(); i++)
	{
		uint8_t pin = Din::ISR_LIST.get(i)->get_pin();
		if(pin == gpio_pin)
		{					
			Din::ISR_LIST.get(i)->call_isr();		
		}
	}
}

void Din::enable_interrupt(void)
{
	if(*_PORT == PORTB)
	{
		Din::PORTB_enable_interrupt();
		PCMSK0 |= (1 << _PIN);
	}
	if(*_PORT == PORTC)
	{
		Din::PORTC_enable_interrupt();
		PCMSK1 |= (1 << _PIN);
	}
	if(*_PORT == PORTD)
	{
		Din::PORTD_enable_interrupt();
		PCMSK2 |= (1 << _PIN);
	}
}

void Din::PORTB_enable_interrupt()
{
	PCICR |= (1 << PCIE0);
}
void Din::PORTC_enable_interrupt()
{
	PCICR |= (1 << PCIE1);
}
void Din::PORTD_enable_interrupt()
{
	PCICR |= (1 << PCIE2);
}

Dout::Dout(){}
Dout::~Dout(){
	//#todo:
}
/*Dout::Dout& operator=(const Dout& t)
{
	
	return *this;
}*/
//copy constructor but it is not used
//non-static reference member can't use default assignment operator
/*Dout::Dout(Dout& t)
{
	_PORT = t._PORT;
	_PIN = t._PIN;
	_PINgroup = t._PINgroup;
	_DDR = t._DDR;
}*/
Dout::Dout(volatile uint8_t* PORT, uint8_t PIN) : GPIObase(PORT, PIN)
{
	*_DDR |= (1 << _PIN);
	Dout::write(GPIO_PIN_RESET);
}
Dout::Dout(volatile uint8_t* PORT, uint8_t PIN, GPIO_PinState state) : GPIObase(PORT, PIN)
{
	*_DDR |= (1 << _PIN);
	Dout::write(state);
}
void Dout::toggle(void){
	*_PORT ^= (1 << _PIN);
}

void Dout::write(GPIO_PinState state)
{
	if (state == GPIO_PIN_SET)	GPIObase::pull_up();
	else GPIObase::pull_down();
}
/*
void Dout::set(void)
{
	*_PORT |= (1 << _PIN);
}
void Dout::clear(void)
{
	*_PORT &= ~(1 << _PIN);
}
*/