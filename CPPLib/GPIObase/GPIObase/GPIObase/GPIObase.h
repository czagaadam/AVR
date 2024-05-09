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

//function pointer for callbacks
typedef void(*gpio_isr_cb)(void);

class GPIObase
{
	public:
	uint8_t read(void);
	void PullUp(void);
	void PullDown(void);	
	uint8_t get_pin(void);
	protected:
	GPIObase();
	GPIObase(volatile uint8_t* PORT, uint8_t PIN);
	~GPIObase();	
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
	~Din();	
	static void trigger_pin(uint8_t PIN);	//find Din object in storage by pin number and call call_isr method of that object
	void set_isr_cb(gpio_isr_cb cb);		//set function pointer
	void call_isr(void);					//call call back function pointed by function pointer
	gpio_isr_cb _cb;						//function pointer
	protected:
	static ISRbase<Din> ISR_LIST;			//storage for Din objects 
	private:
};

class Dout : public GPIObase
{
	public:
	Dout();
	Dout(volatile uint8_t* PORT, uint8_t PIN);
	~Dout();
	void set(void);
	void clear(void);
	void toggle(void);
	protected:
	private:
};

#endif /* GPIOBASE_H_ */