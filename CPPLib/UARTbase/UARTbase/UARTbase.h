/*
 * UARTbase.h
 *
 * Created: 2024. 05. 07. 15:03:56
 *  Author: acer1
 */ 


#ifndef UARTBASE_H_
#define UARTBASE_H_

typedef enum UART_TypeDef
{
	USART0 = 0,
	USART1,
	USART2
}UART_TypeDef;

class UARTbase
{
	public:
	UARTbase();
	UARTbase(UART_TypeDef PORT);
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
	void interrupt_init(void);	
	protected:
	
	private:
	UART_TypeDef _PORT;
};


#endif /* UARTBASE_H_ */