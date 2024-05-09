

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
	if(!isInit)
	{
		if(*_PORT == PORTB)
		{
			_DDR = &DDRB;
			_PINgroup = &PINB;
		}
		if(*_PORT == PORTC)
		{
			_DDR = &DDRC;
			_PINgroup = &PINC;
		}
		if(*_PORT == PORTD)
		{
			_DDR = &DDRD;
			_PINgroup = &PIND;
		}
		isInit = true;
	}
}
uint8_t GPIObase::read(void)	
{
	//#todo:
	//if(!(PINB & (1 << PORTB0))) return 0; else return 1;
	
	//if ( ( ( (*_PINgroup) & ( 1 << _PIN ) ) >> _PIN ) == 0 ) return 0;  else return 1;
	//if ( ( ( (*_PINgroup) & ( 1 << _PIN ) ) >> _PIN ) == 0 ) return 0;  else return 1;
	
	if(*_PORT == PORTB)
	{
		if ( ( ( (PINB) & ( 1 << _PIN ) ) >> _PIN ) == 0 ) return 0;  else return 1;
	}
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
uint8_t GPIObase::get_pin(void){
	return _PIN;
}
Din::Din(){}
Din::~Din(){Din::ISR_LIST.remove(this);}
Din::Din(volatile uint8_t* PORT, uint8_t PIN) : GPIObase(PORT, PIN)
{
	*_DDR &= ~(1 << _PIN);
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

Dout::Dout(){}
Dout::~Dout(){
	//#todo:
}
Dout::Dout(volatile uint8_t* PORT, uint8_t PIN) : GPIObase(PORT, PIN)
{
	*_DDR |= (1 << PIN);
}
void Dout::toggle(void){
	*_PORT ^= (1 << _PIN);
}
void Dout::set(void)
{
	*_PORT |= (1 << _PIN);
}
void Dout::clear(void)
{
	*_PORT &= ~(1 << _PIN);
}
