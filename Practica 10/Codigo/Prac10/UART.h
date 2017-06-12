#ifndef _UART_H
#define _UART_H
#include <inttypes.h>

void UART0_Ini(void);
void UART0_AutoBaudRate(void);
unsigned char UART0_available(void);
unsigned char UART_getchar();
void UART_putchar(unsigned char data);
unsigned int atoi( char *str);
void itoa( char *str,unsigned int num,unsigned char base);
void UART_puts( char *str);
void UART_gets( char *str);


#endif
