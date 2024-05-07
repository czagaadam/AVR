/*
 * UARTbase.h
 *
 * Created: 2024. 05. 07. 15:03:56
 *  Author: acer1
 */ 


#ifndef UARTBASE_H_
#define UARTBASE_H_


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