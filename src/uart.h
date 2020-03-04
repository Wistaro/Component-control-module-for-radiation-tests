/*
 * uart.h
 *
 *  Created on: 22 févr. 2020
 *      Author: WilliamR
 */

#ifndef UART_H_
#define UART_H_

#define UCA0TXD_PIN     GPIO_PORT_P3,GPIO_PIN3 //TX external pin
#define UCA0RXD_PIN     GPIO_PORT_P3,GPIO_PIN4 //RX external pin

#define UCA1TXD_PIN     GPIO_PORT_P4,GPIO_PIN4 //TX for backchannel to USB driver
#define UCA1RXD_PIN     GPIO_PORT_P4,GPIO_PIN5 //RX for backchannel to USB driver

#define TRIG_IF_UART1 __even_in_range(UCA1IV,4)==2 //trig the correct interrupt flag in the interruption routine

uint8_t receivedDataOnUart1;
char flagDataReceivedOnUart1;

#define COMMAND_GET "get"
#define COMMAND_SET "set"
#define COMMAND_RESET "rst"
#define COMMAND_DELATCH "dlt"
#define COMMAND_UNLATCH "rlt"

#define NL 13
#define CR 10

void initUart();
char sendByteUart1(uint8_t input);
char sendStringUart1(uint8_t *strInput);
char interfaceListener();
char handleCommandFromInterface();
char** str_split( char* str, char delim, int* numSplits );
void itoa(unsigned int value, char* result, int base);
void sendFormatedData(char* prefix, char* value);
void cleanTab();

uint8_t indexStringUart;



#endif /* UART_H_ */
