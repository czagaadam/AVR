/*
 * UARTbase.h
 *
 * Created: 2024. 05. 07. 15:03:56
 *  Author: acer1
 */ 


#ifndef UARTBASE_H_
#define UARTBASE_H_
#include "GPIObase/GPIObase/GPIObase/ISRbase.h"

typedef enum UART_TypeDef
{
	UART0 = 0,
	UART1,
	UART2
}UART_TypeDef;

//function pointer for callbacks
typedef void(*uart_isr_cb)(void);

class UARTbase
{
	public:
	UARTbase();
	UARTbase(UART_TypeDef PORT);
	UARTbase(UART_TypeDef PORT, uart_isr_cb cb);
	~UARTbase();
	void init(void);
	void send_byte(uint8_t u8Data);
	void new_line();
	void send_string(char* sData);
	void send_int(uint16_t iData);
	void send_int(uint16_t iData, char* &buffer);
	void send_int_with_text(uint16_t iData, char Text[]);
	void send_int_with_text(uint16_t iData, char* &buffer, char Text[]);
	void send_float(float data);
	uint8_t receive_byte();
	void enable_interrupt(void);	
	static void trigger_port(UART_TypeDef PORT);	//find UART object in storage by UART_TypeDef and call call_isr method of that object
	void set_isr_cb(uart_isr_cb cb);				//set function pointer
	void call_isr(void);							//call call back function pointed by function pointer
	uart_isr_cb _cb;								//function pointer
	UART_TypeDef get_port();
	protected:
	static ISRbase<UARTbase> ISR_LIST;				//storage for UART objects
	
	private:
	UART_TypeDef _PORT;
};


#endif /* UARTBASE_H_ */