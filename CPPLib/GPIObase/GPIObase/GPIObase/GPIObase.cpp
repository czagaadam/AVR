

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
	
	/*char buffer[10];
	itoa(PIN,buffer,10);
	USART0SendString("PIN: ");
	USART0SendString(buffer);
	USART0SendString("\r\n");*/
}
void Din::set_isr_cb(gpio_isr_cb cb)
{
	//USART0SendString("set");
	_cb = cb;	//store (callback) function pointer
	Din::ISR_LIST.add(this);	// IT IS STORING JUST A COPY!!!!!!!!
	/*
	char buffer2[10];
	sprintf(buffer2, "%p",  (void*)this->_cb);
	USART0SendString(buffer2);
	*/	
	}
void Din::call_isr(void)
{
	/*
	USART0SendString("call");		
	char buffer2[10];
	sprintf(buffer2, "%p",  (void*)this->_cb);
	USART0SendString(buffer2);			
	//char buffer[10];
	//itoa(PIN,buffer,10);
	USART0SendString("Name: ");
	strcpy(name,buffer2);
	USART0SendString(name);
	USART0SendString("\r\n");			
*/
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
				/*
				char buffer[10];
				itoa(gpio_pin,buffer,10);
				USART0SendString("TRIGGER PIN: ");
				USART0SendString(buffer);
				USART0SendString("\r\n");	
				
				char buffer2[10];
				sprintf(buffer2, "%p",  (void*)Din::ISR_LIST.get(i)->_cb);
				//char buffer[10];
				//itoa(PIN,buffer,10);
				USART0SendString("Name: ");
				USART0SendString(Din::ISR_LIST.get(i)->name);
				USART0SendString("\r\n");				
				*/						
			Din::ISR_LIST.get(i)->call_isr();		
		}
	}
}

Dout::Dout(){}
Dout::~Dout(){}
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
