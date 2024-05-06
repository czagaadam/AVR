/*
 * GPIObase.h
 *
 * Created: 2024. 04. 25. 12:08:45
 *  Author: AMD_FX_X8
 */ 


#ifndef GPIOBASE_H_
#define GPIOBASE_H_
#include <avr/io.h>
#include "ISRbase.h"

//function pointer for a void function
typedef void(*gpio_isr_cb)(void);

class GPIObase
{
	public:
	uint8_t read(void);
	uint8_t chk(void);
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
/*
class DinStorage {
	private:
	static const int MAX_DIN_COUNT = 24; // Maximum Din elemek száma
	Din dinArray[MAX_DIN_COUNT]; // A hagyományos tömb, amiben tároljuk a Din elemeket
	int dinCount; // Jelenleg tárolt Din elemek száma

	public:
	DinStorage();

	void add(const Din& din);
	
	Din get(int index);
	
	void remove(const Din& din);

	// További függvények (pl. removeDin, getDin, clearDin) implementálása szükség esetén.
};
*/


class Din : public GPIObase
{
	public:
	Din();
	Din(volatile uint8_t* PORT, uint8_t PIN);
	~Din();	
	static void trigger_pin(uint8_t PIN);
	void set_isr_cb(gpio_isr_cb cb);
	void call_isr(void);	
	gpio_isr_cb _cb;
	static ISRbase<Din> ISR_LIST;
	protected:
	
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


	//friend void INT0_vect(void);
	//void twi_interrupt_handler();

/*class DISR : public Din{
	public:
	DISR(volatile uint8_t* PORT, uint8_t PIN);
	~DISR();

	//setter function for (callback) function pointer
	void set_isr_cb(gpio_isr_cb cb);
	void call_isr(void);

	static void trigger_pin(uint16_t gpio_pin);

	private:
	gpio_isr_cb _cb;
	static ISRbase<DISR> ISR_LIST;
};*/
/*typedef enum GPIObase_state
{
	RESET = 0u,
	SET
} GPIObase_state;*/

//ISR(INT0_vect);

/*ISR<GPIObase> DISR::ISR_LIST;

ISR(PCINT0_vect){
	if(!(PINB & (1<<PINB7))) //If PINB7 is low
	{
		LED_ON;
	}
	else
	{
		LED_OFF;
	}
	
}*/

//function pointer for a void function
//typedef void(*gpio_isr_cb)(void);
/*class vector
{
	public:
	vector(void);
	~vector(void);
	void clear(void);
	void add(Din din);
	void remove(void);
	void get(void);
	uint8_t size();
	uint8_t _SIZE;
	static uint8_t index;
	
	protected:
	
	
	private:
	static Din* list[10];
};*/





#endif /* GPIOBASE_H_ */