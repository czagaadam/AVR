/*
 * GPIObase.cpp
 *
 * Created: 2024. 04. 25. 12:08:12
 * Author : AMD_FX_X8
 */ 

#include "GPIObase.h"
#include "ISRbase.h"

ISRbase<Din> Din::ISR_LIST;

GPIObase::GPIObase(){}
GPIObase::GPIObase(volatile uint8_t* PORT, uint8_t PIN)
{
	_PIN = PIN;
	_PORT = PORT;
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
}
GPIObase::~GPIObase(){}	
uint8_t GPIObase::read(void)	
{
	
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
void GPIObase::PullUp(void)
{
	*_PORT |= (1 << _PIN);
}
void GPIObase::PullDown(void)
{
	*_PORT &= ~(1 << _PIN);
}
uint8_t GPIObase::get_pin(void){
	return _PIN;
}
/*
DinStorage::DinStorage() : dinCount(0) {
		// Konstruktor inicializáció
	}
void DinStorage::add(const Din& din) {
	if (dinCount < MAX_DIN_COUNT) {
		dinArray[dinCount++] = din;
		} else {
	}
}
Din DinStorage::get(int index) {
	if (index >= 0 && index < dinCount) {
		return dinArray[index];
		} else {
		return Din();
	}
}
void DinStorage::remove(const Din& din) {
	if (dinCount < MAX_DIN_COUNT) {
		dinArray[dinCount++] = din;
		} else {
	}
}
*/
Din::Din(){}
Din::~Din(){Din::ISR_LIST.remove(this);}
Din::Din(volatile uint8_t* PORT, uint8_t PIN) : GPIObase(PORT, PIN)
{
	/*if(*_PORT == PORTB)
	{
		DDRB &= ~(1 << _PIN);
	}
	if(*_PORT == PORTC)
	{
		DDRC &= ~(1 << _PIN);
	}
	if(*_PORT == PORTD)
	{
		DDRD &= ~(1 << _PIN);
	}*/
	*_DDR &= ~(1 << _PIN);
	Din::ISR_LIST.add(this);
}
void Din::set_isr_cb(gpio_isr_cb cb)
{
	_cb = cb;	//store (callback) function pointer
}
void Din::call_isr(void)
{
	//if(_cb == null)return;
	_cb();
}
void Din::trigger_pin(uint8_t gpio_pin)
{
	for(uint8_t i = 0; i < /*Din::ISR_LIST.size()*/2; i++)
	{
		uint8_t pin = Din::ISR_LIST.get(i)->get_pin();
		if(pin == gpio_pin)
		{
			Din::ISR_LIST.get(i)->call_isr();
		}
	}
}

Dout::Dout(){}
Dout::~Dout(){}
Dout::Dout(volatile uint8_t* PORT, uint8_t PIN) : GPIObase(PORT, PIN)
{
	/*if(*_PORT == PORTB)
	{
		DDRB |= (1 << PIN);
	}
	if(*_PORT == PORTC)
	{
		DDRC |= (1 << PIN);
	}
	if(*_PORT == PORTD)
	{
		DDRD |= (1 << PIN);
	}*/
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





/*
vector::vector(void)  	{
	_SIZE = 24;
	index = 0;
	//list = (Din*)malloc(_SIZE * sizeof(Din));
	//list = new Din[_SIZE];
}
vector::~vector(void)	{}
void vector::clear(void)	{}
void add(Din din) {
	vector::list[vector::index++] = din;
	
}
void vector::remove(void)	{}
void vector::get(void)	{}
uint8_t vector::size(void)	{ return index;}*/