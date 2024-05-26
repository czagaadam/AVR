/*
 * ISRbase.h
 *
 *  Author: Adam Czaga czagaadam@gmail.com
 *
 *  GPIO parent class and Din,Dout inherited classes
 *
 */ 


#ifndef GPIOBASE_H_
#define GPIOBASE_H_
#include <avr/io.h>
#include "ISRbase.h"

typedef enum GPIO_PinState
{
	GPIO_PIN_RESET = 0,
	GPIO_PIN_SET
}GPIO_PinState;

typedef enum GPIO_Pull
{
	GPIO_PULL_DOWN = 0,
	GPIO_PULL_UP
}GPIO_Pull;

//function pointer for callbacks
typedef void(*gpio_isr_cb)(void);

class GPIObase
{
	public:
	//uint8_t read(void);
	GPIO_PinState read(void);
	void pull_up(void);
	void pull_down(void);	
	uint8_t get_pin(void);
	void init(void);
	GPIObase();
	GPIObase(volatile uint8_t* PORT, uint8_t PIN);
	~GPIObase();
	protected:
	uint8_t _PIN;
	volatile uint8_t* _PORT;
	volatile uint8_t* _DDR;
	volatile uint8_t* _PINgroup;	
	private:
};

class Din : public GPIObase
{
	public:
	Din();
	Din(volatile uint8_t* PORT, uint8_t PIN);
	Din(volatile uint8_t* PORT, uint8_t PIN, GPIO_Pull pull);
	Din(volatile uint8_t* PORT, uint8_t PIN, GPIO_Pull pull, gpio_isr_cb cb);
	~Din();	
	static void trigger_pin(uint8_t PIN);	//find Din object in storage by pin number and call call_isr method of that object
	void set_isr_cb(gpio_isr_cb cb);		//set function pointer
	void call_isr(void);					//call call back function pointed by function pointer
	gpio_isr_cb _cb;						//function pointer
	void enable_interrupt(void);	
	protected:
	static ISRbase<Din> ISR_LIST;			//storage for Din objects 
	static void PORTB_enable_interrupt();
	static void PORTC_enable_interrupt();
	static void PORTD_enable_interrupt();	
	private:
};

class Dout : public GPIObase
{
	public:
	Dout();
	Dout(volatile uint8_t* PORT, uint8_t PIN);
	Dout(volatile uint8_t* PORT, uint8_t PIN, GPIO_PinState state);
	~Dout();
	//void set(void);
	//void clear(void);
	void write(GPIO_PinState state);
	void toggle(void);
	protected:
	private:
};

#endif /* GPIOBASE_H_ */