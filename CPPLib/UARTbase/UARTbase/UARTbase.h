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
	UARTbase(UART_TypeDef PORT);
	~UARTbase();
	void Init(void);
	void SendByte(uint8_t u8Data);
	void SendString(char* sData);
	void SendInt(uint16_t iData);
	void SendInt(uint16_t iData, char* &buffer);
	void SendIntWithText(uint16_t iData, char Text[]);
	void SendIntWithText(uint16_t iData, char* &buffer, char Text[]);
	uint8_t ReceiveByte();
	void _Interrupt_Init(void);	
	protected:
	
	private:
	UART_TypeDef _PORT;
};


void USART0Init(void);

void USART0SendByte(uint8_t u8Data);

void USART0SendString(char* sData);

void USART0SendInt(uint16_t iData);

void USART0SendInt(uint16_t iData, char* &buffer);

void USART0SendIntWithText(uint16_t iData, char Text[]);

void USART0SendIntWithText(uint16_t iData, char* &buffer, char Text[]);

uint8_t USART0ReceiveByte();

void USART0_Interrupt_Init(void);


#endif /* UARTBASE_H_ */